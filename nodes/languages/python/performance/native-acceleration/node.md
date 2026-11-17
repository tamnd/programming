---
tag: BKVYYC
kind: feature
owner: python
slug: native-acceleration
name: Native acceleration
aliases: [native code, C extension, NumPy vectorization, vectorization, Cython, mypyc, Numba, PyO3, maturin, Rust extension]
area: languages/python/performance
level: advanced
summary: >-
  A hot loop in Python runs each step in the interpreter. Move the loop into native code: NumPy for arrays, Numba for numeric loops, Cython or mypyc to compile modules, and PyO3 for Rust.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3/extending/index.html", locator: "Extending and Embedding the Python Interpreter, Recommended third party tools"}
see_also: [python/optimization-guidance, python/the-profiling-workflow, python/the-cost-model-of-cpython, python/the-jit-compiler]
sources:
  - url: https://numpy.org/doc/stable/user/whatisnumpy.html
    title: What is NumPy?
    locator: Why is NumPy fast?
    accessed: 2026-11-16
  - url: https://cython.readthedocs.io/en/stable/src/quickstart/overview.html
    title: Cython, an overview
    locator: overview
    accessed: 2026-11-16
  - url: https://mypyc.readthedocs.io/en/stable/introduction.html
    title: "mypyc: Introduction"
    locator: Introduction
    accessed: 2026-11-16
  - url: https://numba.readthedocs.io/en/stable/user/5minguide.html
    title: A ~5 minute guide to Numba
    locator: Will Numba work for my code?; How does Numba work?
    accessed: 2026-11-16
  - url: https://pyo3.rs/latest/
    title: PyO3 user guide
    locator: Introduction; Usage
    accessed: 2026-11-16
  - url: https://docs.python.org/3/extending/index.html
    title: Extending and Embedding the Python Interpreter
    locator: Recommended third party tools
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/sys.html
    title: sys, System-specific parameters and functions
    locator: sys.settrace
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#runtime
    title: Frequently Asked Questions (FAQ)
    locator: Does Go have a runtime?
    accessed: 2026-11-16
---

## Synopsis

```python run id=fallback
def dot(a, b):
    try:
        import numpy as np
    except ImportError:
        return float(sum(x * y for x, y in zip(a, b)))
    return float(np.dot(np.asarray(a, dtype=float), np.asarray(b, dtype=float)))

print(dot([1.0, 2.0, 3.0], [4.0, 5.0, 6.0]))
```

```output for=fallback
32.0
```

CPython runs each step of a Python loop in its interpreter, and each value in the loop is a Python object. Native code runs the same loop as machine code on raw values. To speed up a hot loop, move it into native code. NumPy runs whole-array operations in compiled C. Numba compiles numeric functions to machine code when they are called. Cython and mypyc compile a Python module to a C extension. PyO3 builds an extension module in Rust. The example uses NumPy when it is installed, and a pure-Python loop when it is not. Both paths give the same result.

## Semantics

**r1.** NumPy describes the cost of a loop in Python: "if a and b each contain millions of numbers, we will pay the price for the inefficiencies of looping in Python."[^1] A loop in C "saves all the overhead involved in interpreting the Python code and manipulating Python objects".[^1] Example: [`interpreter-steps`](#interpreter-steps).

**r2.** "Vectorization describes the absence of any explicit looping, indexing, etc., in the code - these things are taking place, of course, just “behind the scenes” in optimized, pre-compiled C code."[^1] In NumPy, `c = a * b` "does what the earlier examples do, at near-C speeds, but with the code simplicity we expect from something based on Python."[^1]

**r3.** "Numba is a just-in-time compiler for Python that works best on code that uses NumPy arrays and functions, and loops."[^4] "Numba reads the Python bytecode for a decorated function and combines this with information about the types of the input arguments to the function. It analyzes and optimizes your code, and finally uses the LLVM compiler library to generate a machine code version of your function, tailored to your CPU capabilities."[^4] Code that uses pandas is an example that Numba does not help: "Pandas is not understood by Numba and as a result Numba would simply run this code via the interpreter but with the added cost of the Numba internal overheads!"[^4]

**r4.** "[Cython] is a programming language that makes writing C extensions for the Python language as easy as Python itself."[^2] "Its main feature on top of these is support for optional static type declarations as part of the language. The source code gets translated into optimized C/C++ code and compiled as Python extension modules."[^2]

**r5.** "Mypyc compiles Python modules to C extensions. It uses standard Python type hints to generate fast code."[^3] Its documentation says: "Existing code with type annotations is often 1.5x to 5x faster when compiled. Code tuned for mypyc can be 5x to 10x faster."[^3] "Mypyc enforces type annotations (and type comments) at runtime".[^3]

**r6.** PyO3 has "Rust bindings for Python, including tools for creating native Python extension modules."[^5] "maturin is a tool for building and publishing Rust-based Python packages with minimal configuration."[^5]

**r7.** The Python documentation covers the C API, and says: "Some third party tools offer both simpler and more sophisticated approaches to creating C and C++ extensions for Python."[^6]

## Examples

`sys.settrace` calls a trace function with the event `'line'` when "The interpreter is about to execute a new line of code or re-execute the condition of a loop."[^7] A loop in Python makes about 2 line events for each item. The built-in `sum` with `operator.mul` runs the whole loop in C, and makes 1 line event (rule r1):

```python run id=interpreter-steps
import operator
import sys

def dot_loop(a, b):
    total = 0
    for i in range(len(a)):
        total += a[i] * b[i]
    return total

def dot_builtin(a, b):
    return sum(map(operator.mul, a, b))

def count_lines(f, *args):
    count = 0
    def tracer(frame, event, arg):
        nonlocal count
        if event == 'line':
            count += 1
        return tracer
    sys.settrace(tracer)
    try:
        result = f(*args)
    finally:
        sys.settrace(None)
    return result, count

a = list(range(1000))
b = list(range(1000))
r1, n1 = count_lines(dot_loop, a, b)
r2, n2 = count_lines(dot_builtin, a, b)
print('same result:', r1 == r2)
print('Python loop:', n1, 'line events')
print('built-in:', n2, 'line event')
print(type(sum).__name__, type(dot_loop).__name__)
```

```output for=interpreter-steps
same result: True
Python loop: 2003 line events
built-in: 1 line event
builtin_function_or_method function
```

## Pitfalls

Profile first. Native code helps only the part of the program that runs in it. If the time goes to I/O or to the database, a compiled loop does not help.

Each call from Python into native code, and each conversion of a Python object to a native value, has a cost. A native function that does little work for each call can be slower than the Python code. Give the native code a whole array or a whole loop, not one item at a time (rule r2).

Numba compiles a function the first time it is called with new argument types, so the first call is slow. Code that uses objects that Numba does not understand, such as pandas data frames, gets no gain (rule r3).

mypyc checks the type annotations when the code runs (rule r5). Code that passes values of other types than its annotations say can fail after it is compiled.

A C extension or a Rust extension must be built for each platform and each Python version that the package supports. This makes the release of the package harder than the release of pure Python.

## Comparison

The Go FAQ says: "Go programs are compiled ahead of time to native machine code".[^8] A Go loop is native code, so Go has no need to move a hot loop into another language. In Python, the interpreter runs each step of a loop (rule r1), so a program moves its hot loops into code that a compiler made: NumPy, Numba, Cython, mypyc or an extension in Rust.

## References

[^1]: [What is NumPy?](https://numpy.org/doc/stable/user/whatisnumpy.html)
[^2]: [Cython, an overview](https://cython.readthedocs.io/en/stable/src/quickstart/overview.html)
[^3]: [mypyc: Introduction](https://mypyc.readthedocs.io/en/stable/introduction.html)
[^4]: [A ~5 minute guide to Numba](https://numba.readthedocs.io/en/stable/user/5minguide.html)
[^5]: [PyO3 user guide](https://pyo3.rs/latest/)
[^6]: [Extending and Embedding the Python Interpreter](https://docs.python.org/3/extending/index.html)
[^7]: [sys, System-specific parameters and functions](https://docs.python.org/3/library/sys.html)
[^8]: [Frequently Asked Questions (FAQ)](https://go.dev/doc/faq#runtime)
