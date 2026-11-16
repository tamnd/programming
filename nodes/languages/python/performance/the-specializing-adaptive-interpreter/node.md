---
tag: Q08ZKX
kind: feature
owner: python
slug: the-specializing-adaptive-interpreter
name: The specializing adaptive interpreter
aliases: [PEP 659, specialization, quickening, inline cache, adaptive bytecode, type stability, Faster CPython, superinstruction]
area: languages/python/performance
level: advanced
summary: >-
  CPython replaces a generic bytecode instruction with a specialized one for the types that it sees, and caches data inline in the bytecode. When the types change, the instruction adapts again.
grammar: null
spec: {standard: null, url: "https://peps.python.org/pep-0659/", locator: "PEP 659, Specializing Adaptive Interpreter"}
see_also: [python/the-cost-model-of-cpython, python/performance-measurement, just-in-time-compilation]
sources:
  - url: https://peps.python.org/pep-0659/
    title: "PEP 659: Specializing Adaptive Interpreter"
    locator: Abstract; Performance; Quickening; Ancillary data
    accessed: 2026-11-16
  - url: https://docs.python.org/3/whatsnew/3.11.html#whatsnew311-pep659
    title: What’s New In Python 3.11
    locator: Faster CPython; PEP 659, Specializing Adaptive Interpreter
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/dis.html
    title: dis, Disassembler for Python bytecode
    locator: introduction; Bytecode analysis
    accessed: 2026-11-16
  - url: https://go.dev/doc/pgo
    title: Profile-guided optimization
    locator: Overview
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#runtime
    title: Frequently Asked Questions (FAQ)
    locator: Does Go have a runtime?
    accessed: 2026-11-16
---

## Synopsis

```python run id=specialize
import dis

class Point:
    def __init__(self, x):
        self.x = x

def work(p, n):
    return abs(p.x + n)

def ops(f):
    return [i.opname for i in dis.get_instructions(f, adaptive=True)
            if i.opname.startswith(('BINARY_OP', 'LOAD_ATTR', 'LOAD_GLOBAL'))]

print(ops(work))
for i in range(1000):
    work(Point(i), i)
print(ops(work))
for i in range(1000):
    work(Point(i * 0.5), 0.5)
print(ops(work))
```

```output for=specialize
['LOAD_GLOBAL', 'LOAD_ATTR', 'BINARY_OP']
['LOAD_GLOBAL_BUILTIN', 'LOAD_ATTR_INSTANCE_VALUE', 'BINARY_OP_ADD_INT']
['LOAD_GLOBAL_BUILTIN', 'LOAD_ATTR_INSTANCE_VALUE', 'BINARY_OP_ADD_FLOAT']
```

CPython runs bytecode. When a function runs many times, CPython replaces some generic instructions with specialized instructions for the types that it sees. In the example, `BINARY_OP` becomes `BINARY_OP_ADD_INT` while the function adds integers, and `BINARY_OP_ADD_FLOAT` after it adds floats. `LOAD_GLOBAL` becomes `LOAD_GLOBAL_BUILTIN`, because `abs` is a builtin. `dis` with `adaptive=True` shows the specialized instructions. The names of these instructions are details of CPython, and they can change in each version.

## Semantics

**r1.** "In order to perform well, virtual machines for dynamic languages must specialize the code that they execute to the types and values in the program being run. This specialization is often associated with “JIT” compilers, but is beneficial even without machine code generation."[^1]

**r2.** "A specializing, adaptive interpreter is one that speculatively specializes on the types or values it is currently operating on, and adapts to changes in those types and values."[^1] Example: [`specialize`](#specialize).

**r3.** "The general idea is that while Python is a dynamic language, most code has regions where objects and types rarely change. This concept is known as type stability."[^2] "Python will only specialize when it sees code that is “hot” (executed multiple times). This prevents Python from wasting time on run-once code. Python can also de-specialize when code is too dynamic or when the use changes."[^2]

**r4.** "Quickening is the process of replacing slow instructions with faster variants."[^1] "Each instruction that would benefit from specialization is replaced by an adaptive version during quickening."[^1]

**r5.** Specialized instructions keep data in the bytecode. The PEP calls this "a form of inline cache, an “inline data cache”".[^1] For a global name, "The object’s index in the globals/builtins namespace is cached. Loading globals and builtins require zero namespace lookups."[^2] Example: [`caches`](#caches).

**r6.** "The specializer will also combine certain common instruction pairs into one superinstruction, reducing the overhead during execution."[^2]

**r7.** "The adaptive bytecode can be shown by passing adaptive=True."[^3] "Bytecode is an implementation detail of the CPython interpreter. No guarantees are made that bytecode will not be added, removed, or changed between versions of Python."[^3]

**r8.** For PEP 659, "Speedups seem to be in the range 10% - 60%. Most of the speedup comes directly from specialization. The largest contributors are speedups to attribute lookup, global variables, and calls."[^1] "CPython 3.11 is an average of 25% faster than CPython 3.10 as measured with the pyperformance benchmark suite, when compiled with GCC on Ubuntu Linux. Depending on your workload, the overall speedup could be 10-60%."[^2] PEP 659 is one of the parts of this work.[^2]

## Examples

The code object holds more 16-bit code units than it has instructions. The extra units are the inline caches (rule r5):

```python run id=caches
import dis

def work(n):
    return abs(n + 1)

units = len(work.__code__.co_code) // 2
instructions = len(list(dis.get_instructions(work)))
print(units > instructions)
```

```output for=caches
True
```

The specialization is a change in the bytecode, not in the result. A function gives the same result before and after it is specialized (rules r2 and r7):

```python run id=same-result
def add(a, b):
    return a + b

before = [add(1, 2), add('a', 'b'), add([1], [2])]
for i in range(1000):
    add(i, i)
after = [add(1, 2), add('a', 'b'), add([1], [2])]
print(before == after, after)
```

```output for=same-result
True [3, 'ab', [1, 2]]
```

## Pitfalls

Do not depend on the names of the specialized instructions, or on the number of calls before a function is specialized (rule r7). They are details of CPython, and they change between versions.

Code that keeps one type at one place in the program is the code that benefits (rule r3). An instruction that sees many types must adapt again, and the generic path is slower.

The speedups in rule r8 are averages over benchmarks. Measure your own program with `timeit` or a profiler.

Other implementations, such as PyPy, do not use this interpreter. The facts on this page are for CPython (rule r7).

## Comparison

The Go FAQ says: "Go programs are compiled ahead of time to native machine code".[^5] Go can use a profile from earlier runs at build time: "Profile-guided optimization (PGO), also known as feedback-directed optimization (FDO), is a compiler optimization technique that feeds information (a profile) from representative runs of the application back into to the compiler for the next build of the application, which uses that information to make more informed optimization decisions."[^4] CPython collects the same kind of information while the program runs, and it changes the bytecode at once (rule r2).

## References

[^1]: [PEP 659: Specializing Adaptive Interpreter](https://peps.python.org/pep-0659/)
[^2]: [What’s New In Python 3.11](https://docs.python.org/3/whatsnew/3.11.html#whatsnew311-pep659)
[^3]: [dis, Disassembler for Python bytecode](https://docs.python.org/3/library/dis.html)
[^4]: [Profile-guided optimization](https://go.dev/doc/pgo)
[^5]: [Frequently Asked Questions (FAQ)](https://go.dev/doc/faq#runtime)
