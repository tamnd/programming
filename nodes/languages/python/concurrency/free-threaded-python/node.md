---
tag: NEW-python-free-threaded-python
kind: feature
owner: python
slug: free-threaded-python
name: Free-threaded Python
aliases: [free threading, nogil, GIL-free Python, python3.14t, python3.13t, PEP 703, PEP 779, Py_GIL_DISABLED, PYTHON_GIL, --disable-gil]
area: languages/python/concurrency
level: advanced
summary: >-
  The free-threaded build of CPython runs without the global interpreter lock, so threads run Python code in parallel. It came in 3.13 as an experiment and is a supported, optional build from 3.14.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/howto/free-threading-python.html", locator: "Python support for free threading"}
proposals: [pep-703, pep-779]
since: "3.13"
see_also: [python/the-global-interpreter-lock, python/atomicity-and-the-memory-model, python/multi-threading, python/thread-synchronization]
sources:
  - url: https://docs.python.org/3.15/howto/free-threading-python.html
    title: Python support for free threading, Python 3.15 documentation
    locator: the introduction; Identifying free-threaded Python; The global interpreter lock in free-threaded Python; Thread safety
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/whatsnew/3.13.html#free-threaded-cpython
    title: What’s New In Python 3.13
    locator: Free-threaded CPython
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/threadsafety.html
    title: Thread Safety Guarantees, Python 3.15 documentation
    locator: Thread safety for list objects; Thread safety for dict objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/howto/free-threading-python.html#known-limitations
    title: Python support for free threading, Python 3.15 documentation
    locator: Known limitations; Behavioral changes
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/whatsnew/3.14.html#free-threaded-mode-improvements
    title: What’s New In Python 3.14
    locator: Free-threaded mode improvements
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0779/
    title: "PEP 779: Criteria for supported status for free-threaded Python"
    locator: Abstract; Motivation
    accessed: 2026-11-16
---

## Synopsis

```python run id=which-build
import sys
import sysconfig

print(sysconfig.get_config_var("Py_GIL_DISABLED"))
print("free-threading build" in sys.version)
print(sys._is_gil_enabled())
```

```output for=which-build
0
False
True
```

This output comes from the default build. On a free-threaded build such as `python3.14t`, the first line is 1, the second is `True`, and the GIL is off unless something turns it on again.

## Semantics

**r1.** CPython "has support for a build of Python called free threading where the global interpreter lock (GIL) is disabled." Such a build runs "threads in parallel on available CPU cores."[^1]

**r2.** The free-threaded build is a separate executable. In 3.13 it is "usually called python3.13t or python3.13t.exe", and a source build uses the `--disable-gil` option.[^2]

**r3.** The configuration variable `Py_GIL_DISABLED` tells the build: "If the variable is set to 1, then the build supports free threading." `sys._is_gil_enabled()` tells whether the GIL "is actually disabled in the running process."[^1] Example: [`which-build`](#which-build).

**r4.** A free-threaded build can run with the GIL on. The environment variable `PYTHON_GIL` or the option `-X gil` turns it on. "The GIL may also automatically be enabled when importing a C-API extension module that is not explicitly marked as supporting free threading."[^1]

**r5.** "Built-in types like dict, list, and set use internal locks to protect against concurrent modifications in ways that behave similarly to the GIL." The guide still recommends "threading.Lock or other synchronization primitives instead of relying on the internal locks of built-in types".[^1] Example: [`portable-lock`](#portable-lock).

**r6.** The Thread Safety Guarantees page lists the rules for each type. For example, `lst.append(x)` and `lst.pop()` are atomic, but "Operations that involve multiple accesses, as well as iteration, are never atomic."[^3]

**r7.** "It is generally not thread-safe to access the same iterator object from multiple threads concurrently, and threads may see duplicate or missing elements."[^4]

**r8.** The build changes some defaults. In it, "the flag thread_inherit_context is set to true by default", so a new `threading.Thread` starts with a copy of the context of the caller of `start()`. In the default build, a thread starts with an empty context.[^4] Example: [`inherit-context`](#inherit-context).

**r9.** The build "has additional overhead when executing Python code compared to the default GIL-enabled build", and it "will typically use more memory".[^4]

## Examples

```python run id=portable-lock
import threading

totals = {"hits": 0}
lock = threading.Lock()

def hit(times):
    for _ in range(times):
        with lock:
            totals["hits"] += 1

threads = [threading.Thread(target=hit, args=(5_000,)) for _ in range(4)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print(totals)
```

```output for=portable-lock
{'hits': 20000}
```

`totals["hits"] += 1` is a read, an add and a write, so the internal lock of the dict does not make it atomic (rule r6). With the explicit lock, the program prints the same total on both builds.

```python run id=inherit-context
import contextvars
import sys
import threading

user = contextvars.ContextVar("user", default="nobody")
user.set("alice")

def show():
    print("in the thread:", user.get())

print("inherit:", bool(sys.flags.thread_inherit_context))
t = threading.Thread(target=show)
t.start()
t.join()

t = threading.Thread(target=show, context=contextvars.copy_context())
t.start()
t.join()
```

```output for=inherit-context
inherit: False
in the thread: nobody
in the thread: alice
```

The default build does not copy the context, so the first thread sees the default value. A thread that gets `context=` sees the value on both builds (rule r8).

## Versions

> [!since v="3.13"]
> The free-threaded build is available as an experiment, "not enabled by default", with "a substantial single-threaded performance hit."[^2]

> [!since v="3.14"]
> The implementation of PEP 703 is finished, and the specializing adaptive interpreter works in free-threaded mode. "The performance penalty on single-threaded code in free-threaded mode is now roughly 5-10%".[^5] PEP 779, for 3.14, sets the criteria for the next phase: "Phase II would make the free-threaded build officially supported but still optional, and phase III would make the free-threaded build the default."[^6]

## Pitfalls

An extension module that does not declare support turns the GIL on again at import (rule r4). A program then runs with the GIL, and threads give no speedup. Check `sys._is_gil_enabled()` after the imports.

Code that was safe only because of the GIL can fail. Two threads that share an iterator can skip or repeat items (rule r7). A check followed by an action, such as `if key in d: del d[key]`, is not atomic; the guarantees page suggests `d.pop(key, None)` instead.[^3]

A thread in the free-threaded build sees the context variables of its parent, and a thread in the default build does not (rule r8). Pass `context=` to `Thread` when the code must behave the same on both builds.

## Comparison

In Go, Java and C#, threads always run in parallel, and the programmer writes the locks. Free-threaded Python moves CPython toward that model. Its built-in types keep internal locks, so a data race corrupts no list or dict, but the order of the program's own steps is still the job of its locks (rules r5 and r6).
