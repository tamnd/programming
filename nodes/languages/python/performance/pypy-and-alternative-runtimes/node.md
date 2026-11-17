---
tag: HDYZK7
kind: feature
owner: python
slug: pypy-and-alternative-runtimes
name: PyPy and alternative runtimes
aliases: [PyPy, GraalPy, RPython, tracing JIT, meta-tracing, cpyext, alternative Python implementation, sys.implementation, platform.python_implementation]
area: languages/python/performance
level: advanced
summary: >-
  PyPy and GraalPy run Python code with a JIT compiler and can be much faster than CPython for long pure-Python programs. C extensions run slower or are experimental, and objects are not freed at once.
grammar: null
spec: {standard: null, url: "https://pypy.org/features.html", locator: "PyPy, Features"}
see_also: [python/the-jit-compiler, python/native-acceleration, python/memory-management-in-cpython, just-in-time-compilation]
sources:
  - url: https://pypy.org/features.html
    title: PyPy, Features
    locator: introduction; Speed; Memory usage
    accessed: 2026-11-17
  - url: https://pypy.org/compat.html
    title: Python compatibility
    locator: TL;DR; Refcounting, __del__, and resource use
    accessed: 2026-11-17
  - url: https://rpython.readthedocs.io/en/latest/jit/overview.html
    title: Motivating JIT Compiler Generation
    locator: Overview; The path we followed
    accessed: 2026-11-17
  - url: https://doc.pypy.org/en/latest/faq.html
    title: Frequently Asked Questions
    locator: Should I install numpy or numpypy?
    accessed: 2026-11-17
  - url: https://github.com/oracle/graalpython
    title: GraalPy, the GraalVM Implementation of Python
    locator: README
    accessed: 2026-11-17
  - url: https://docs.python.org/3/library/sys.html
    title: sys, System-specific parameters and functions
    locator: sys.implementation
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/platform.html
    title: platform, Access to underlying platform’s identifying data
    locator: platform.python_implementation
    accessed: 2026-11-17
  - url: https://pypy.org/
    title: PyPy
    locator: home page
    accessed: 2026-11-17
  - url: https://go.dev/doc/faq#What_compiler_technology_is_used_to_build_the_compilers
    title: Frequently Asked Questions (FAQ)
    locator: What compiler technology is used to build the compilers?
    accessed: 2026-11-16
---

## Synopsis

```python run id=which
import platform
import sys

print(sys.implementation.name)
print(platform.python_implementation())
print(sys.implementation.name == platform.python_implementation().lower())
```

```output for=which
cpython
CPython
True
```

Python is a language with more than one implementation. CPython is the reference implementation, and the output above comes from CPython. PyPy and GraalPy are other implementations, and each has a JIT compiler. They can run a long pure-Python program much faster than CPython. They are less compatible with C extensions, and they free objects at a different time. A program can read the name of the implementation from `sys.implementation.name` or from `platform.python_implementation()`.

## Semantics

**r1.** "PyPy is a replacement for CPython. It is built using the RPython language that was co-developed with it. The main reason to use it instead of CPython is speed".[^1] The PyPy home page says: "On average, PyPy 3.12 is about 4 times faster than CPython 3.11."[^8]

**r2.** PyPy does not have a JIT compiler that someone wrote by hand. "We indeed wrote an interpreter for Python, but we never wrote any JIT compiler for Python in PyPy. Instead, we use the fact that our interpreter for Python is written in RPython, which is a nice, high-level language – and we turn it automatically into a JIT compiler for Python."[^3] The generator "is instead based on the notion of tracing JIT, recently studied for Java and JavaScript."[^3] This JIT "generates machine code at runtime, aggressively optimizing the user’s program and leading to a big performance boost, while keeping the semantics unmodified."[^3]

**r3.** PyPy names two cases that it cannot speed up: "Short-running processes: if it doesn't run for at least a few seconds, then the JIT compiler won't have enough time to warm up."[^1] And: "If all the time is spent in run-time libraries (i.e. in C functions), and not actually running Python code, the JIT compiler will not help."[^1]

**r4.** "Pure python code works, but there are a few differences with object lifetime management. Modules that use the CPython C API will probably work, but will not achieve a speedup via the JIT."[^2] NumPy runs on PyPy through the `cpyext` layer, and "The main drawback of numpy is that cpyext is infamously slow".[^4]

**r5.** PyPy "does not support refcounting semantics" to release state at once when the program drops the last reference to an object.[^2] A file that the program does not close is closed later, when the garbage collector runs. "The proper fix is" a `with` statement.[^2] Example: [`lifetime`](#lifetime).

**r6.** "GraalPy is a high-performance implementation of the Python language for the JVM".[^5] Its README says: "GraalPy is ready for production running pure Python code and has experimental support for many popular native extension modules."[^5] "Pure Python code is often faster than on CPython after JIT compilation".[^5] "It has first-class support for embedding in Java and can turn Python applications into fast, standalone binaries."[^5]

**r7.** `sys.implementation` is "An object containing information about the implementation of the currently running Python interpreter."[^6] Its `name` "is the implementation’s identifier, e.g. 'cpython'."[^6] `platform.python_implementation()` "Returns a string identifying the Python implementation."[^7] Example: [`which`](#which).

## Examples

Code that closes its resources with `with`, and that closes generators that it does not finish, behaves the same on each implementation (rule r5). Code that waits for the garbage collector to close them does not:

```python run id=lifetime
import os
import tempfile

events = []

def numbers():
    try:
        yield 1
        yield 2
    finally:
        events.append('generator cleaned up')

path = os.path.join(tempfile.mkdtemp(), 'out.txt')
with open(path, 'w') as f:
    f.write('stuff')
events.append(f'file closed: {f.closed}')

gen = numbers()
for n in gen:
    if n == 1:
        break
gen.close()
events.append('after the loop')
print('\n'.join(events))
```

```output for=lifetime
file closed: True
generator cleaned up
after the loop
```

## Pitfalls

Measure your own program on the other runtime before you switch. The average speedups in rules r1 and r6 come from benchmark suites, and a program that spends its time in C code, or runs for less than a few seconds, can get no gain (rule r3).

Check each dependency. A package with C extensions can be slower on PyPy, and support for native extensions in GraalPy is experimental (rules r4 and r6).

Do not depend on the time when an object is freed. Close files, sockets and generators in a `with` statement or with an explicit call, not in `__del__` (rule r5).

An alternative runtime implements a version of the language that can be older than the newest CPython. Check which Python version the runtime supports before you use new syntax.

Do not test for the implementation in normal code. Use `sys.implementation.name` only for a known difference, such as a workaround for one runtime (rule r7).

## Comparison

Go also has more than one compiler. The Go FAQ says: "There are several production compilers for Go, and a number of others in development for various platforms. The default compiler, gc, is included with the Go distribution as part of the support for the go command."[^9] "The Gccgo compiler is a front end written in C++ with a recursive descent parser coupled to the standard GCC back end."[^9] Each Go compiler compiles ahead of time. The Python runtimes differ more: CPython interprets bytecode, and PyPy and GraalPy compile hot code while the program runs (rules r2 and r6).

## References

[^1]: [PyPy, Features](https://pypy.org/features.html)
[^2]: [Python compatibility](https://pypy.org/compat.html)
[^3]: [Motivating JIT Compiler Generation](https://rpython.readthedocs.io/en/latest/jit/overview.html)
[^4]: [Frequently Asked Questions](https://doc.pypy.org/en/latest/faq.html)
[^5]: [GraalPy, the GraalVM Implementation of Python](https://github.com/oracle/graalpython)
[^6]: [sys, System-specific parameters and functions](https://docs.python.org/3/library/sys.html)
[^7]: [platform, Access to underlying platform’s identifying data](https://docs.python.org/3/library/platform.html)
[^8]: [PyPy](https://pypy.org/)
[^9]: [Frequently Asked Questions (FAQ)](https://go.dev/doc/faq#What_compiler_technology_is_used_to_build_the_compilers)
