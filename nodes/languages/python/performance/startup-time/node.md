---
tag: 18GKNV
kind: feature
owner: python
slug: startup-time
name: Startup time
aliases: [startup time, -X importtime, PYTHONPROFILEIMPORTTIME, frozen modules, lazy imports, lazy import, PEP 810, importlib.util.LazyLoader, site module, -S]
area: languages/python/performance
level: advanced
summary: >-
  Most of the startup time of a Python program is imports. Measure them with -X importtime, skip the site module with -S when possible, and defer heavy imports with lazy imports or LazyLoader.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3/using/cmdline.html", locator: "Command line and environment, -X importtime"}
see_also: [python/performance-measurement, python/the-profiling-workflow, python/optimization-guidance]
sources:
  - url: https://docs.python.org/3/using/cmdline.html
    title: Command line and environment
    locator: -S; -X importtime; -X frozen_modules; -X lazy_imports
    accessed: 2026-11-16
  - url: https://docs.python.org/3/whatsnew/3.15.html
    title: What’s new in Python 3.15
    locator: "PEP 810: Explicit lazy imports"
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0810/
    title: "PEP 810: Explicit lazy imports"
    locator: Abstract; Motivation
    accessed: 2026-11-16
  - url: https://docs.python.org/3/whatsnew/3.11.html
    title: What’s New In Python 3.11
    locator: Faster Startup, Frozen imports / Static code objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/site.html
    title: site, Site-specific configuration hook
    locator: introduction
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/importlib.html
    title: importlib, The implementation of import
    locator: importlib.util.LazyLoader
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#runtime
    title: Frequently Asked Questions (FAQ)
    locator: Does Go have a runtime?
    accessed: 2026-11-16
---

## Synopsis

```python run id=importtime
import subprocess
import sys

r = subprocess.run([sys.executable, '-X', 'importtime', '-c', 'import json'],
                   capture_output=True, text=True, timeout=60)
lines = r.stderr.splitlines()
print(lines[0].split('|')[0].strip())
json_line = next(line for line in lines if line.split('|')[-1].strip() == 'json')
self_us, cumulative_us = (int(f.split(':')[-1]) for f in json_line.split('|')[:2])
print('cumulative >= self:', cumulative_us >= self_us)
```

```output for=importtime
import time: self [us]
cumulative >= self: True
```

Before a Python program runs its first line, the interpreter starts, imports the `site` module, and then runs the imports of the program. A short program, such as a command-line tool, can spend most of its time in imports. The option `-X importtime` prints the time of each import to standard error, in microseconds. The example runs a child process with this option and reads the line for `json`.

## Semantics

**r1.** An import does real work: "when a module is imported, Python must locate the file, read it from disk, compile it to bytecode, and execute all top-level code. For applications with deep dependency trees, this process can take seconds, even when most of the imported code is never actually used during a particular run."[^2]

**r2.** "-X importtime to show how long each import takes. It shows module name, cumulative time (including nested imports) and self time (excluding nested imports)."[^1] "Typical usage is python -X importtime -c 'import asyncio'."[^1] Example: [`importtime`](#importtime).

**r3.** The `site` module "is automatically imported during initialization. The automatic import can be suppressed using the interpreter’s -S option."[^5] The option `-S` disables "the import of the module site and the site-dependent manipulations of sys.path that it entails."[^1] Without `site`, the directories of third-party packages are not on `sys.path`. Example: [`site`](#site).

**r4.** The core modules of startup are frozen. "In Python 3.11, the core modules essential for Python startup are “frozen”. This means that their Code Objects (and bytecode) are statically allocated by the interpreter."[^4] "Interpreter startup is now 10-15% faster in Python 3.11."[^4] "-X frozen_modules determines whether or not frozen modules are ignored by the import machinery."[^1] "The default is on if this is an installed Python (the normal case)."[^1] Example: [`site`](#site).

**r5.** Python 3.15 has lazy imports. "When you mark an import as lazy, Python defers the actual module loading until the imported name is first used."[^2] The syntax is `lazy import json` or `lazy from pathlib import Path`.[^2] "Lazy imports are only permitted at module scope; using lazy inside a function, class body, or try/except/finally block raises a SyntaxError."[^2]

**r6.** "In the case where loading a lazily imported module fails (for example, if the module does not exist), Python raises the exception at the point of first use rather than at import time."[^2]

**r7.** "For cases where you want to enable lazy loading globally without modifying source code, Python provides the -X lazy_imports command-line option and the PYTHON_LAZY_IMPORTS environment variable."[^2] For command-line tools, PEP 810 says: "This can reduce startup time by 50-70% in practice".[^3]

**r8.** Earlier versions of Python have other ways to defer an import. An `import` statement in a function runs only when the function runs. `importlib.util.LazyLoader` is "A class which postpones the execution of the loader of a module until the module has an attribute accessed."[^6] Example: [`deferred`](#deferred).

## Examples

With `-S`, the interpreter does not import `site`, and it imports fewer modules (rules r3 and r4). The module `os` comes from the frozen modules in an installed Python:

```python run id=site
import subprocess
import sys

code = 'import os, sys; print(len(sys.modules), "site" in sys.modules, os.__spec__.origin)'

def start(*flags):
    r = subprocess.run([sys.executable, *flags, '-c', code],
                       capture_output=True, text=True, timeout=60)
    n, has_site, origin = r.stdout.split()
    return int(n), has_site, origin

normal, no_site = start(), start('-S')
print('site imported:', normal[1], no_site[1])
print('fewer modules with -S:', no_site[0] < normal[0])
print('os comes from:', normal[2])
```

```output for=site
site imported: True False
fewer modules with -S: True
os comes from: frozen
```

A module runs its top-level code when the program imports it. An import in a function, and an import through `LazyLoader`, run it later (rule r8):

```python run id=deferred
import importlib.util
import os
import sys
import tempfile

folder = tempfile.mkdtemp()
for name in ('heavy_a', 'heavy_b'):
    with open(os.path.join(folder, name + '.py'), 'w') as f:
        f.write(f'print("{name} runs")\nVALUE = 42\n')
sys.path.insert(0, folder)

def use_a():
    import heavy_a
    return heavy_a.VALUE

print('program starts')
print('use_a returns', use_a())

spec = importlib.util.find_spec('heavy_b')
spec.loader = importlib.util.LazyLoader(spec.loader)
heavy_b = importlib.util.module_from_spec(spec)
sys.modules['heavy_b'] = heavy_b
spec.loader.exec_module(heavy_b)
print('heavy_b is imported')
print('heavy_b.VALUE is', heavy_b.VALUE)
```

```output for=deferred
program starts
heavy_a runs
use_a returns 42
heavy_b is imported
heavy_b runs
heavy_b.VALUE is 42
```

## Pitfalls

A deferred import moves the cost. It does not remove it. If the program uses the module in each run, the total time stays the same, and the cost comes at the first use.

A deferred import also moves the errors. A module that does not exist fails at the first use, not at the start of the program (rule r6). Tests must reach the first use to find the error.

Do not use `-S` in a program that needs third-party packages from `site-packages`. Without `site`, Python does not add these directories to `sys.path` (rule r3).

The numbers of `-X importtime` change from run to run, because the disk cache and the load on the machine change. Run it more than once. The first run after a change can also include the time to compile the module to bytecode.

A module that does work at import time, such as reading files or opening connections, makes each import slow. Move such work into a function that the program calls when it needs the result.

## Comparison

The Go FAQ says: "Go programs are compiled ahead of time to native machine code".[^7] A Go program does not find, read and compile its packages when it starts. A Python program does this work for each module that it imports (rule r1), so imports are the main part of its startup time.

## References

[^1]: [Command line and environment](https://docs.python.org/3/using/cmdline.html)
[^2]: [What’s new in Python 3.15](https://docs.python.org/3/whatsnew/3.15.html)
[^3]: [PEP 810: Explicit lazy imports](https://peps.python.org/pep-0810/)
[^4]: [What’s New In Python 3.11](https://docs.python.org/3/whatsnew/3.11.html)
[^5]: [site, Site-specific configuration hook](https://docs.python.org/3/library/site.html)
[^6]: [importlib, The implementation of import](https://docs.python.org/3/library/importlib.html)
[^7]: [Frequently Asked Questions (FAQ)](https://go.dev/doc/faq#runtime)
