---
tag: 0XV4VQ
kind: feature
owner: python
slug: eval-and-exec
name: eval, exec and compile
aliases: [eval, exec, compile, code object, ast.literal_eval, literal_eval, dynamic code, globals and locals, __builtins__]
area: languages/python/metaprogramming
level: advanced
summary: >-
  eval runs a string as an expression and returns its value. exec runs statements. compile turns source into a code object for either one. They run arbitrary code, so never pass them untrusted input.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/functions.html#exec", locator: "Built-in Functions: compile(), eval(), exec()"}
proposals: []
see_also: [python/dynamic-class-creation, python/scope-and-dynamic-features, python/python-scopes-and-namespaces, python/the-ast-module]
sources:
  - url: https://docs.python.org/3.15/builtins/functions.html#exec
    title: Built-in Functions, Python 3.15 documentation
    locator: compile(); eval(); exec()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/ast.html#ast.literal_eval
    title: ast — Abstract syntax trees, Python 3.15 documentation
    locator: ast.literal_eval()
    accessed: 2026-11-16
---

## Synopsis

```python run id=basic
x = 10
print(eval("x * 2 + 1"))
print(eval("a + b", {"a": 1, "b": 2}))

ns = {}
exec("def double(n):\n    return n * 2\nresult = double(21)", ns)
print(ns["result"], "double" in ns)
```

```output for=basic
21
3
42 True
```

## Semantics

**r1.** `eval(source, globals, locals)` parses the source "as a Python expression (technically speaking, an expression list) using the globals and locals mappings as global and local namespace." It returns the value.[^1] Example: [`basic`](#basic).

**r2.** `exec(source, globals, locals)` parses a string "as a suite of Python statements which is then executed". It returns `None`. "If only globals is provided, it must be a dictionary (and not a subclass of dictionary), which will be used for both the global and the local variables."[^1]

**r3.** `compile(source, filename, mode)` turns source into a code object. The mode "can be 'exec' if source consists of a sequence of statements, 'eval' if it consists of a single expression, or 'single' if it consists of a single interactive statement".[^1] Example: [`compile-once`](#compile-once).

**r4.** "When exec gets two separate objects as globals and locals, the code will be executed as if it were embedded in a class definition." So functions in that code cannot see the top-level names.[^1] Example: [`split-namespaces`](#split-namespaces).

**r5.** Without a `locals` argument in a function, changes that `exec` makes to local names are not visible after it returns. "Pass an explicit locals dictionary if you need to see effects of the code on locals after function exec() returns."[^1] Example: [`function-locals`](#function-locals).

**r6.** "This function executes arbitrary code. Calling it with untrusted user-supplied input will lead to security vulnerabilities."[^1] A custom `__builtins__` "is not a security mechanism: the executed code can still access all builtins."[^1] Example: [`no-sandbox`](#no-sandbox).

**r7.** `ast.literal_eval` accepts only "strings, bytes, numbers, tuples, lists, dicts, sets, booleans, None and Ellipsis." It "is specifically designed not to execute Python code, unlike the more general eval()". But it "is not free from attack", and "Calling it on untrusted data is thus not recommended."[^2] Example: [`literal-eval`](#literal-eval).

## Examples

```python run id=compile-once
code = compile("n ** 2", "<formula>", "eval")
print([eval(code, {"n": n}) for n in range(4)])

block = compile("total = sum(range(5))", "<block>", "exec")
g = {}
print(eval(block, g), g["total"])
```

```output for=compile-once
[0, 1, 4, 9]
None 10
```

The formula is parsed once and run four times. A code object made in `'exec'` mode gives `None` from `eval()` (rule r3).

```python run id=literal-eval
import ast

print(ast.literal_eval("{'a': [1, 2.5], 'b': (True, None)}"))
try:
    ast.literal_eval("__import__('os').getcwd()")
except ValueError:
    print("rejected")
```

```output for=literal-eval
{'a': [1, 2.5], 'b': (True, None)}
rejected
```

`literal_eval` reads data, not code. The call in the second string is not allowed (rule r7).

```python run id=function-locals
def f():
    loc = {}
    exec("y = 5", globals(), loc)
    print(loc["y"])

f()
```

```output for=function-locals
5
```

## Versions

> [!since v="3.13"]
> `globals` and `locals` can be keyword arguments, and the default locals of `exec()` and `eval()` in a function follow the new rules of `locals()`.[^1]

## Pitfalls

```python fails id=eval-statement expect=exception:SyntaxError
eval("x = 1")
```

`eval` takes an expression, and `x = 1` is a statement. Use `exec` for statements (rules r1 and r2).

```python fails id=split-namespaces expect=exception:NameError
exec("""
z = 3

def get_z():
    return z

print(get_z())
""", {}, {})
```

With two separate dictionaries, `z` goes into the locals dictionary, as a class variable would. The function looks for `z` in the globals and does not find it (rule r4). Pass one dictionary.

```python wrong id=no-sandbox
safe = {"__builtins__": {}}
print(eval("().__class__.__mro__[1].__name__", safe))
```

```output for=no-sandbox
object
```

The empty `__builtins__` removed `len` and `open` as names, but the code still reached `object` through an attribute chain. From there it can reach much more. Do not use `eval` on untrusted input (rule r6).

## Comparison

`eval` and `exec` make Python act as its own interpreter at run time. Python has no macros, so code that writes code builds a string or an AST (with [[python/the-ast-module]]) and compiles it. See [[syntactic-macros]] for languages that change code at compile time.
