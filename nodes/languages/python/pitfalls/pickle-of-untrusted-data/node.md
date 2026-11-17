---
tag: G637BG
kind: feature
owner: python
slug: pickle-of-untrusted-data
name: pickle of untrusted data
aliases: [unpickling untrusted data, pickle.loads security, pickle arbitrary code execution, __reduce__ exploit, CWE-502, S301, suspicious-pickle-usage, find_class]
area: languages/python/pitfalls
level: core
summary: >-
  pickle.loads can run any code that the data names, so a pickle from an untrusted source is a remote code execution risk. Use JSON for such data, and sign pickles with hmac when you must store them.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/pickle.html", locator: "pickle, Warning"}
proposals: []
see_also: [python/pickle-and-object-serialization, python/saving-structured-data-with-json, python/cryptographic-services, python/eval-of-untrusted-input, python/file-format-modules, deserialization-of-untrusted-data]
sources:
  - url: https://docs.python.org/3.15/library/pickle.html
    title: pickle, Python object serialization, Python 3.15 documentation
    locator: Warning; Comparison with json; Pickling Class Instances; Restricting Globals
    accessed: 2026-11-18
  - url: https://cwe.mitre.org/data/definitions/502.html
    title: CWE-502, Deserialization of Untrusted Data
    locator: Description
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/hmac.html
    title: hmac, Keyed-Hashing for Message Authentication, Python 3.15 documentation
    locator: hmac.compare_digest()
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/suspicious-pickle-usage/
    title: suspicious-pickle-usage (S301), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
---

## Synopsis

```python wrong id=code-on-load
import pickle


class Payload:
    def __reduce__(self):
        return (print, ("this ran inside pickle.loads",))


data = pickle.dumps(Payload())
result = pickle.loads(data)
print(result)
```

```output for=code-on-load
this ran inside pickle.loads
None
```

The data told `pickle.loads` to call `print`. A real attack names a function such as `os.system` in the same way, and the call runs on the computer that loads the data.

## Semantics

**r1.** The documentation is direct: "The pickle module is not secure. Only unpickle data you trust."[^1] "It is possible to construct malicious pickle data which will execute arbitrary code during unpickling. Never unpickle data that could have come from an untrusted source, or that could have been tampered with."[^1]

**r2.** A pickle is a small program for the loader, not only data. "By default, unpickling will import any class or function that it finds in the pickle data. For many applications, this behaviour is unacceptable as it permits the unpickler to import and invoke arbitrary code."[^1]

**r3.** The method `__reduce__` controls how an object is saved. It can return a tuple whose first item is "A callable object that will be called to create the initial version of the object."[^1] The loader calls that callable with the saved arguments. Example: [`code-on-load`](#code-on-load). The data is the attack, so the attacker does not need the class on the loading computer.

**r4.** This is CWE-502: "The product deserializes untrusted data without sufficiently ensuring that the resulting data will be valid."[^2] For the general class of flaw, see [[deserialization-of-untrusted-data]].

**r5.** Fix 1, use a data format for data from outside. "Safer serialization formats such as json may be more appropriate if you are processing untrusted data."[^1] "Unlike pickle, deserializing untrusted JSON does not in itself create an arbitrary code execution vulnerability."[^1] Example: [`json-instead`](#json-instead).

**r6.** Fix 2, sign pickles that the program stores and loads itself. "Consider signing data with hmac if you need to ensure that it has not been tampered with."[^1] Check the signature before `pickle.loads`, and compare with `hmac.compare_digest`, because the documentation recommends it "instead of the == operator to reduce the vulnerability to timing attacks."[^3] Example: [`signed-pickle`](#signed-pickle).

**r7.** A restricted unpickler can limit the names. "you may want to control what gets unpickled by customizing Unpickler.find_class(). Unlike its name suggests, Unpickler.find_class() is called whenever a global (i.e., a class or a function) is requested. Thus it is possible to either completely forbid globals or restrict them to a safe subset."[^1] This reduces the risk, but the safe subset must be small and checked. Example: [`restricted`](#restricted).

**r8.** Ruff S301 "Checks for calls to pickle functions or modules that wrap them."[^4] Its reason: "Deserializing untrusted data with pickle and other deserialization modules is insecure as it can allow for the creation of arbitrary objects, which can then be used to achieve arbitrary code execution and otherwise unexpected behavior."[^4]

## Examples

```python run id=json-instead
import json

text = json.dumps({"user": "ann", "scores": [3, 5]})
print(text)
print(json.loads(text)["scores"])
```

```output for=json-instead
{"user": "ann", "scores": [3, 5]}
[3, 5]
```

JSON gives only dicts, lists, strings, numbers, `True`, `False` and `None`, and it calls no function that the data names (rule r5).

```python run id=signed-pickle
import hashlib
import hmac
import pickle

KEY = b"a secret key that only the server has"


def dump_signed(obj):
    body = pickle.dumps(obj)
    tag = hmac.new(KEY, body, hashlib.sha256).digest()
    return tag + body


def load_signed(blob):
    tag, body = blob[:32], blob[32:]
    good = hmac.new(KEY, body, hashlib.sha256).digest()
    if not hmac.compare_digest(tag, good):
        raise ValueError("bad signature")
    return pickle.loads(body)


blob = dump_signed({"cart": ["tea"]})
print(load_signed(blob))

forged = blob[:32] + pickle.dumps({"cart": ["free tea"]})
try:
    load_signed(forged)
except ValueError as e:
    print(e)
```

```output for=signed-pickle
{'cart': ['tea']}
bad signature
```

The changed data fails the check, so `pickle.loads` never sees it (rule r6). The key must stay secret. A person with the key can sign any pickle.

## Versions

The risk is the same in all Python versions, because it follows from rules r2 and r3.

## Pitfalls

```python fails id=restricted expect=exception:UnpicklingError
import io
import pickle

SAFE = {("builtins", "set"), ("builtins", "frozenset")}


class SafeUnpickler(pickle.Unpickler):
    def find_class(self, module, name):
        if (module, name) in SAFE:
            return super().find_class(module, name)
        raise pickle.UnpicklingError(f"global '{module}.{name}' is forbidden")


class Payload:
    def __reduce__(self):
        return (print, ("this must not run",))


print(SafeUnpickler(io.BytesIO(pickle.dumps({1, 2}))).load())
SafeUnpickler(io.BytesIO(pickle.dumps(Payload()))).load()
```

The first load works, because a set is in the safe list. The second load asks for `builtins.print`, so `find_class` raises `UnpicklingError`, and the call does not run (rule r7).

```python wrong id=load-is-enough
import pickle


class Payload:
    def __reduce__(self):
        return (print, ("ran, and the result was never used",))


blob = pickle.dumps(Payload())
del Payload

pickle.loads(blob)
```

```output for=load-is-enough
ran, and the result was never used
```

The class is gone, and the program does not use the result, but the call still runs. A check of the result after `loads` is too late (rule r3).

## Comparison

In Python, `pickle` can save almost any object, because the loader can import and call code (rule r2). That power is the risk. A format such as JSON can hold less, but it is safe to read from any source (rule r5). For pickle in general, see [[python/pickle-and-object-serialization]]. For a similar risk with text, see [[python/eval-of-untrusted-input]].
