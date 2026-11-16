---
tag: KFNS7V
kind: feature
owner: python
slug: the-jit-compiler
name: The JIT compiler
aliases: [JIT, just-in-time compiler, PEP 744, copy-and-patch, stencils, Tier 2, micro-ops, uops, PYTHON_JIT, sys._jit, --enable-experimental-jit]
area: languages/python/performance
level: research
summary: >-
  CPython has an experimental JIT compiler. It turns hot traces of micro-ops into machine code with copy-and-patch. It is a build option, and PYTHON_JIT turns it on or off when Python starts.
grammar: null
spec: {standard: null, url: "https://peps.python.org/pep-0744/", locator: "PEP 744, JIT Compilation"}
see_also: [python/the-specializing-adaptive-interpreter, just-in-time-compilation, python/performance-measurement]
sources:
  - url: https://peps.python.org/pep-0744/
    title: "PEP 744: JIT Compilation"
    locator: Abstract; Rationale; Specification
    accessed: 2026-11-16
  - url: https://docs.python.org/3/whatsnew/3.13.html
    title: What’s New In Python 3.13
    locator: An experimental just-in-time (JIT) compiler
    accessed: 2026-11-16
  - url: https://docs.python.org/3/whatsnew/3.14.html
    title: What’s new in Python 3.14
    locator: Binary releases for the experimental just-in-time compiler
    accessed: 2026-11-16
  - url: https://docs.python.org/3/whatsnew/3.15.html
    title: What’s new in Python 3.15
    locator: Upgraded experimental JIT compiler
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/sys.html
    title: sys, System-specific parameters and functions
    locator: sys._jit
    accessed: 2026-11-16
  - url: https://arxiv.org/abs/2011.13127
    title: "Copy-and-Patch Compilation: A fast compilation algorithm for high-level languages and bytecode"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#runtime
    title: Frequently Asked Questions (FAQ)
    locator: Does Go have a runtime?
    accessed: 2026-11-16
---

## Synopsis

```python run id=jit-state
import sys

jit = getattr(sys, '_jit', None)
if jit is None:
    available = enabled = False
else:
    available, enabled = jit.is_available(), jit.is_enabled()
print(type(available).__name__, type(enabled).__name__)
print('enabled implies available:', not enabled or available)
```

```output for=jit-state
bool bool
enabled implies available: True
```

CPython first runs a function in its bytecode interpreter. When a part of the program gets hot, CPython can translate it to micro-ops, optimize them and, in a build with the JIT, translate them to machine code. The JIT is experimental. A build of CPython has it only when the build turns it on, and the environment variable `PYTHON_JIT` turns it on or off when Python starts. The example reads the state of the JIT. `sys._jit` does not exist in each version and in each build, so the example uses `getattr` and prints only facts that are true in each case.

## Semantics

**r1.** PEP 744 says: "Earlier this year, an experimental “just-in-time” compiler was merged into CPython’s main development branch."[^1] The PEP was created on 11 April 2024, and its status is Draft.[^1]

**r2.** "When CPython is configured and built using the --enable-experimental-jit option, a just-in-time (JIT) compiler is added which may speed up some Python programs."[^2] The option takes the values `no`, `yes`, `yes-off` and `interpreter`.[^2] With `yes`, "To disable the JIT at runtime, pass the environment variable PYTHON_JIT=0."[^2] With `yes-off`, "To enable the JIT at runtime, pass the environment variable PYTHON_JIT=1."[^2] Example: [`jit-off`](#jit-off).

**r3.** "We start with specialized Tier 1 bytecode."[^2] "When the Tier 1 bytecode gets hot enough, it gets translated to a new purely internal intermediate representation (IR), called the Tier 2 IR, and sometimes referred to as micro-ops (“uops”)."[^2] "When the JIT is enabled, the optimized Tier 2 IR is translated to machine code, which is then executed."[^2]

**r4.** "The machine code translation process uses a technique called copy-and-patch. It has no runtime dependencies, but there is a new build-time dependency on LLVM."[^2] The paper on copy-and-patch says that it works "by stitching together code from a large library of binary implementation variants. We call these binary implementations stencils because they have holes where missing values must be inserted during code generation."[^6] Example: [`stencils`](#stencils).

**r5.** "In a nutshell, copy-and-patch allows a high-quality template JIT compiler to be generated from the same DSL used to generate the rest of the interpreter."[^1] "Despite their reputation, JIT compilers are not magic “go faster” machines."[^1]

**r6.** In Python 3.14, "The official macOS and Windows release binaries now include an experimental just-in-time (JIT) compiler. Although it is not recommended for production use, it can be tested by setting PYTHON_JIT=1 as an environment variable."[^3] For 3.14, "the typical performance impact of enabling it can range from 10% slower to 20% faster, depending on workload."[^3] "Free-threaded builds do not support JIT compilation."[^3]

**r7.** For Python 3.15, "Results from the pyperformance benchmark suite report a 7-8% geometric mean performance improvement for the JIT over the standard CPython interpreter built with all optimizations enabled on x86-64 Linux."[^4] "The speedups for JIT builds versus no JIT builds range from roughly 15% slowdown to over 100% speedup (ignoring the unpack_sequence microbenchmark) on x86-64 Linux and AArch64 macOS systems."[^4]

**r8.** The JIT becomes non-experimental when, among other conditions, "It provides a meaningful performance improvement for at least one popular platform (realistically, on the order of 5%)."[^1]

**r9.** `sys._jit` has "Utilities for observing just-in-time compilation."[^5] "JIT compilation is an experimental implementation detail of CPython. sys._jit is not guaranteed to exist or behave the same way in all Python implementations, versions, or build configurations."[^5] `is_enabled()` returns `True` if JIT compilation is enabled for the process, and this "implies sys._jit.is_available()".[^5] Example: [`jit-state`](#jit-state).

## Examples

With `PYTHON_JIT=0`, a child process has no enabled JIT, in each build and in each version (rules r2 and r9):

```python run id=jit-off
import os
import subprocess
import sys

code = "import sys; j = getattr(sys, '_jit', None); print(bool(j and j.is_enabled()))"
env = dict(os.environ, PYTHON_JIT='0')
r = subprocess.run([sys.executable, '-c', code], env=env, capture_output=True, text=True,
                   timeout=30)
print(r.stdout.strip())
```

```output for=jit-off
False
```

A model of copy-and-patch in Python. Each stencil is a piece of code with holes. The compiler copies the stencil of each micro-op and fills the holes with the values of the trace (rule r4). The real stencils are machine code that LLVM makes when CPython is built, not text:

```python run id=stencils
STENCILS = {
    'LOAD_CONST': 'push {value}',
    'LOAD_FAST': 'push local[{value}]',
    'ADD_INT': 'b = pop; a = pop; push a + b',
    'RETURN': 'return pop',
}

trace = [('LOAD_FAST', 0), ('LOAD_CONST', 1), ('ADD_INT', None), ('RETURN', None)]
for op, value in trace:
    print(STENCILS[op].format(value=value))
```

```output for=stencils
push local[0]
push 1
b = pop; a = pop; push a + b
return pop
```

## Pitfalls

Do not expect the JIT to make each program faster. For 3.14, the effect ranged from 10% slower to 20% faster, and for 3.15, from about 15% slower to over 100% faster (rules r6 and r7). Measure your own program with the JIT on and off.

Do not use the JIT in production as if it were stable. It is experimental, and the release notes of 3.14 do not recommend it for production use (rule r6).

Do not depend on `sys._jit` in a library (rule r9). It can be missing, and its behavior can change.

A free-threaded build of 3.14 does not support the JIT (rule r6).

## Comparison

The Go FAQ says: "Go’s runtime does not include a virtual machine, such as is provided by the Java runtime. Go programs are compiled ahead of time to native machine code".[^7] A Go program is machine code before it starts, so Go has no JIT. CPython starts in an interpreter, and its JIT makes machine code only for hot code while the program runs (rule r3).

## References

[^1]: [PEP 744: JIT Compilation](https://peps.python.org/pep-0744/)
[^2]: [What’s New In Python 3.13](https://docs.python.org/3/whatsnew/3.13.html)
[^3]: [What’s new in Python 3.14](https://docs.python.org/3/whatsnew/3.14.html)
[^4]: [What’s new in Python 3.15](https://docs.python.org/3/whatsnew/3.15.html)
[^5]: [sys, System-specific parameters and functions](https://docs.python.org/3/library/sys.html)
[^6]: [Copy-and-Patch Compilation: A fast compilation algorithm for high-level languages and bytecode](https://arxiv.org/abs/2011.13127)
[^7]: [Frequently Asked Questions (FAQ)](https://go.dev/doc/faq#runtime)
