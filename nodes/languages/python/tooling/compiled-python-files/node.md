---
tag: 12FQSD
kind: feature
owner: python
slug: compiled-python-files
name: Compiled Python Files
aliases: [".pyc", __pycache__, bytecode cache, PEP 3147, PEP 552, hash-based pycs, py_compile, compileall, importlib.util.cache_from_source, importlib.util.MAGIC_NUMBER, sys.implementation.cache_tag, PYTHONPYCACHEPREFIX, "-X pycache_prefix", sourceless import, python -B]
area: languages/python/tooling
level: core
summary: >-
  Imported modules are cached as bytecode in __pycache__/name.tag.pyc, checked against the source's mtime and size or a hash. The main script is not cached; compileall precompiles.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/modules.html", locator: "6.1.3. “Compiled” Python files"}
see_also: [python/the-python-interpreter, python/invoking-the-interpreter, python/the-interpreter-and-its-environment, python/development-mode-and-x-options]
sources:
  - url: https://docs.python.org/3.15/tutorial/modules.html
    title: Modules, The Python Tutorial 3.15
    locator: 6.1.3. “Compiled” Python files
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/importlib.html
    title: importlib, Python 3.15 documentation
    locator: importlib.util.MAGIC_NUMBER; cache_from_source; source_from_cache
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/py_compile.html
    title: py_compile, Python 3.15 documentation
    locator: PycInvalidationMode
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0552/
    title: PEP 552, Deterministic pycs
    locator: Specification
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/cmdline.html
    title: Command line and environment, Python 3.15 documentation
    locator: -B; --check-hash-based-pycs; PYTHONPYCACHEPREFIX
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/compileall.html
    title: compileall, Python 3.15 documentation
    locator: Command-line use (-b, --invalidation-mode)
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html
    title: sys, Python 3.15 documentation
    locator: sys.pycache_prefix
    accessed: 2026-11-16
  - url: https://pkg.go.dev/cmd/go
    title: Command go, Go documentation
    locator: Build and test caching
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import os
import pathlib
import subprocess
import sys
import tempfile


# a clean environment: PYTHONDONTWRITEBYTECODE would turn caching off
env = {k: v for k, v in os.environ.items()
       if k not in ('PYTHONDONTWRITEBYTECODE', 'PYTHONPYCACHEPREFIX')}


def cached(tmp):
    cache = pathlib.Path(tmp, '__pycache__')
    tag = sys.implementation.cache_tag
    return sorted(p.name.replace(tag, '<tag>') for p in cache.glob('*.pyc'))


with tempfile.TemporaryDirectory() as tmp:
    pathlib.Path(tmp, 'helper.py').write_text('VALUE = 1\n')
    pathlib.Path(tmp, 'main.py').write_text('import helper\nprint(helper.VALUE)\n')
    for opts in (['-B'], [], ['-m', 'compileall', '-q', '.']):
        args = opts if opts[:1] == ['-m'] else [*opts, 'main.py']
        subprocess.run([sys.executable, *args], cwd=tmp, env=env, check=True, capture_output=True)
        print(f'{" ".join(opts) or "(none)":17}', cached(tmp))
```

```output for=synopsis
-B                []
(none)            ['helper.<tag>.pyc']
-m compileall -q . ['helper.<tag>.pyc', 'main.<tag>.pyc']
```

`main.py` imports `helper`. With `-B` nothing is cached. A normal run caches `helper` but not `main.py`, the script given on the command line. `compileall` compiles both files ahead of time. `<tag>` stands for `sys.implementation.cache_tag`, such as `cpython-312`. The children run without `PYTHONDONTWRITEBYTECODE`, which works like `-B` (r8), so that a caller's setting does not change the result.

## Semantics

**r1.** "To speed up loading modules, Python caches the compiled version of each module in the __pycache__ directory under the name module.version.pyc, where the version encodes the format of the compiled file; it generally contains the Python version number."[^1] "This naming convention allows compiled modules from different releases and different versions of Python to coexist."[^1] `importlib.util.cache_from_source` returns that path for a source path, and `source_from_cache` goes the other way.[^2]

**r2.** "Python checks the modification date of the source against the compiled version to see if it’s out of date and needs to be recompiled. This is a completely automatic process."[^1] A timestamp pyc "includes the timestamp and size of the source file, which Python will compare against the metadata of the source file at runtime".[^3]

**r3.** "Python does not check the cache in two circumstances. First, it always recompiles and does not store the result for the module that’s loaded directly from the command line. Second, it does not check the cache if there is no source module. To support a non-source (compiled only) distribution, the compiled module must be in the source directory, and there must not be a source module."[^1]

**r4.** A pyc file starts with a 16-byte header. "The first word will continue to be the magic number, versioning the bytecode and pyc format. The second word, conceptually the new word, will be a bit field."[^4] "If the bit field is 0, the pyc is a traditional timestamp-based pyc."[^4] "If the lowest bit of the bit field is set, the pyc is a hash-based pyc. We call the second lowest bit the check_source flag."[^4] `importlib.util.MAGIC_NUMBER` is "The bytes which represent the bytecode version number."[^2]

**r5.** Hash-based pycs come in two kinds. `CHECKED_HASH` means the file "includes a hash of the source file content, which Python will compare against the source at runtime to determine if the .pyc file needs to be regenerated."[^3] With `UNCHECKED_HASH`, "Python will at runtime assume the .pyc file is up to date and not validate the .pyc against the source file at all. This option is useful when the .pycs are kept up to date by some system external to Python like a build system."[^3] `--check-hash-based-pycs always` validates both kinds against the source, and `never` validates neither.[^5]

**r6.** "A program doesn’t run any faster when it is read from a .pyc file than when it is read from a .py file; the only thing that’s faster about .pyc files is the speed with which they are loaded."[^1] "The -O switch removes assert statements, the -OO switch removes both assert statements and __doc__ strings", and such files have an `opt-` tag in their names.[^1]

**r7.** `py_compile` provides "a function to generate a byte-code file from a source file".[^3] `compileall` compiles a whole tree; it "can be used to create the cached byte-code files at library installation time, which makes them available for use even by users who don’t have write permission to the library directories."[^6] Its `-b` option means "Write the byte-code files to their legacy locations and names", and `--invalidation-mode` chooses timestamp or hash-based files.[^6]

**r8.** Caching can be turned off or moved. With `-B`, "Python won’t try to write .pyc files on the import of source modules."[^5] With `PYTHONPYCACHEPREFIX`, "Python will write .pyc files in a mirror directory tree at this path, instead of in __pycache__ directories within the source tree. This is equivalent to specifying the -X pycache_prefix=PATH option."[^5] Then "Any __pycache__ directories in the source code tree will be ignored and new .pyc files written within the pycache prefix."[^7]

## Examples

### Cache paths and the header

```python run id=header
import importlib.util
import pathlib
import py_compile
import sys
import tempfile

Mode = py_compile.PycInvalidationMode
with tempfile.TemporaryDirectory() as tmp:
    source = pathlib.Path(tmp, 'm.py')
    source.write_text('X = 1\n')
    target = importlib.util.cache_from_source(str(source))
    print(pathlib.Path(target).relative_to(tmp).as_posix().replace(sys.implementation.cache_tag, '<tag>'))
    print(importlib.util.source_from_cache(target) == str(source))
    for mode in (Mode.TIMESTAMP, Mode.CHECKED_HASH, Mode.UNCHECKED_HASH):
        header = pathlib.Path(py_compile.compile(str(source), invalidation_mode=mode)).read_bytes()[:16]
        print(f'{mode.name:15}', header[:4] == importlib.util.MAGIC_NUMBER,
              int.from_bytes(header[4:8], 'little'))
```

```output for=header
__pycache__/m.<tag>.pyc
True
TIMESTAMP       True 0
CHECKED_HASH    True 3
UNCHECKED_HASH  True 1
```

`cache_from_source` and `source_from_cache` map the two paths onto each other (r1). Each file starts with the magic number. The second word is the bit field: 0 for a timestamp pyc, 3 for hash-based with `check_source`, and 1 for hash-based without it (r4, r5).

### A stale timestamp pyc

```python run id=stale
import os
import pathlib
import py_compile
import subprocess
import sys
import tempfile

Mode = py_compile.PycInvalidationMode
for mode in (Mode.TIMESTAMP, Mode.CHECKED_HASH):
    with tempfile.TemporaryDirectory() as tmp:
        source = pathlib.Path(tmp, 'conf.py')
        source.write_text('LIMIT = 1\n')
        py_compile.compile(str(source), invalidation_mode=mode)
        stat = source.stat()
        source.write_text('LIMIT = 2\n')  # same size
        os.utime(source, ns=(stat.st_atime_ns, stat.st_mtime_ns))  # same mtime
        result = subprocess.run([sys.executable, '-c', 'import conf; print(conf.LIMIT)'],
                                cwd=tmp, capture_output=True, text=True)
        print(f'{mode.name:12}', result.stdout.strip())
```

```output for=stale
TIMESTAMP    1
CHECKED_HASH 2
```

The source changes from `LIMIT = 1` to `LIMIT = 2`, but keeps its size and gets its old modification time back. The timestamp pyc still matches that metadata, so the import uses the old bytecode and sees 1 (r2). The checked hash pyc compares the content, finds a mismatch, and recompiles (r5). Build tools that reset file times, or copy files with their old times, hit this case.

### Running without the source

```python run id=sourceless
import os
import pathlib
import subprocess
import sys
import tempfile

# a clean environment: PYTHONDONTWRITEBYTECODE would turn caching off
env = {k: v for k, v in os.environ.items()
       if k not in ('PYTHONDONTWRITEBYTECODE', 'PYTHONPYCACHEPREFIX')}

with tempfile.TemporaryDirectory() as tmp:
    pathlib.Path(tmp, 'helper.py').write_text('VALUE = 1\n')
    pathlib.Path(tmp, 'main.py').write_text('import helper\nprint(helper.VALUE)\n')

    def run():
        result = subprocess.run([sys.executable, 'main.py'], cwd=tmp, env=env,
                                capture_output=True, text=True)
        return (result.stdout or result.stderr.splitlines()[-1]).strip()

    print('source:', run())
    pathlib.Path(tmp, 'helper.py').unlink()
    print('__pycache__ only:', run())
    pathlib.Path(tmp, 'helper.py').write_text('VALUE = 1\n')
    subprocess.run([sys.executable, '-m', 'compileall', '-q', '-b', 'helper.py'], cwd=tmp, env=env,
                   check=True)
    pathlib.Path(tmp, 'helper.py').unlink()
    print('legacy helper.pyc:', run())
    print(sorted(p.name for p in pathlib.Path(tmp).iterdir()))
```

```output for=sourceless
source: 1
__pycache__ only: ModuleNotFoundError: No module named 'helper'
legacy helper.pyc: 1
['__pycache__', 'helper.pyc', 'main.py']
```

After `helper.py` is deleted, the cached `__pycache__/helper.<tag>.pyc` is not used, because the cache is only read for a module whose source exists (r3). `compileall -b` writes `helper.pyc` next to `main.py`, the legacy place that a source-less import looks in (r7).

### Moving the cache with PYTHONPYCACHEPREFIX

```python run id=prefix
import os
import pathlib
import subprocess
import sys
import tempfile

with tempfile.TemporaryDirectory() as tmp:
    src = pathlib.Path(tmp, 'src')
    src.mkdir()
    (src / 'helper.py').write_text('VALUE = 1\n')
    env = {k: v for k, v in os.environ.items() if k != 'PYTHONDONTWRITEBYTECODE'}
    env['PYTHONPYCACHEPREFIX'] = os.path.join(tmp, 'cache')
    subprocess.run([sys.executable, '-c', 'import helper'], cwd=src, env=env, check=True)
    hidden = os.path.realpath(tmp).lstrip('/')
    for path in sorted(pathlib.Path(tmp).rglob('*.pyc')):
        rel = path.relative_to(tmp).as_posix()
        if '/src/' in rel:
            print(rel.replace(hidden, '<tmp>').replace(sys.implementation.cache_tag, '<tag>'))
    print('src/__pycache__ exists:', (src / '__pycache__').exists())
```

```output for=prefix
cache/<tmp>/src/helper.<tag>.pyc
src/__pycache__ exists: False
```

The pyc for `src/helper.py` goes under `cache/`, at a path that repeats the source directory, and the source tree gets no `__pycache__` (r8). The program prints only that file: the mirror tree also holds the standard modules the interpreter imported at startup.

## Pitfalls

- A `.pyc` file is not a way to hide code. The tutorial notes that it only loads faster (r6), and a `.pyc` holds the full bytecode of the module.
- A deleted module can still seem to exist when a legacy `name.pyc` sits next to the script (r3, r7). Remove stray `.pyc` files along with their sources.
- A pyc in `__pycache__` belongs to one interpreter version through its tag (r1). Copying `__pycache__` to a machine with another version does nothing useful; that interpreter compiles its own.
- When `compileall` is run as a build step and `PYTHONPYCACHEPREFIX` is set at run time, both must use the same prefix (r8), or the prepared files are never read.
- `UNCHECKED_HASH` files are never compared with the source (r5). Edits to the source have no effect until the pyc is rebuilt.

## Comparison

Go compiles ahead of time, so there is no bytecode beside the source, but the go command keeps a build cache: it "caches build outputs for reuse in future builds."[^8] Like a checked hash pyc, "The build cache correctly accounts for changes to Go source files, compilers, compiler options, and so on: cleaning the cache explicitly should not be necessary in typical use."[^8] It lives in one per-user directory, which `GOCACHE` overrides, closer to `PYTHONPYCACHEPREFIX` than to `__pycache__`.[^8]

[^1]: Modules, The Python Tutorial 3.15.
[^3]: py_compile, Python 3.15 documentation.
[^6]: compileall, Python 3.15 documentation.
[^2]: importlib, Python 3.15 documentation.
[^4]: PEP 552, Deterministic pycs.
[^5]: Command line and environment, Python 3.15 documentation.
[^7]: sys, Python 3.15 documentation.
[^8]: Command go, Go documentation.
