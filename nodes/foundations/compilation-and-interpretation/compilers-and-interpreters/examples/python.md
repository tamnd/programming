```python run id=code-objects
source = "x * 2 + 1"
code = compile(source, "<formula>", "eval")  # source text to a code object
print(type(code).__name__, code.co_names)
print([eval(code, {"x": x}) for x in range(4)])  # run it many times


def area(w, h):
    return w * h


print(type(area.__code__).__name__, area.__code__.co_varnames)

namespace = {}
exec("def double(n):\n    return n * 2\n", namespace)  # compile a definition at run time
print(namespace["double"](21))
```

```output for=code-objects
code ('x',)
[1, 3, 5, 7]
code ('w', 'h')
42
```

```python fails expect=compile id=whole-file-compiled-first
print("start")  # never runs: Python compiles the whole file first


def total(xs):
    return sum(xs)


def broken(
    return 0
```

- Verdict: native. CPython compiles each module to bytecode before it runs any of it, caches the bytecode in `.pyc` files, and runs it on a virtual machine. `compile` turns source text into a code object at run time, `eval` and `exec` run it, and every function keeps its compiled code in `__code__`. Compiling once and running the code object many times avoids parsing the text again.
- The fails block has a syntax error on its last line. Because CPython compiles the whole file first, the program stops with `SyntaxError` before the `print` on the first line runs.
