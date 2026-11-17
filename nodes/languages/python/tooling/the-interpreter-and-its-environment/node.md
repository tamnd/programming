---
tag: 4GRB9W
kind: feature
owner: python
slug: the-interpreter-and-its-environment
name: The Interpreter and Its Environment
aliases: [source code encoding, encoding declaration, "# -*- coding: -*-", coding cookie, PEP 263, tokenize.detect_encoding, PYTHONPATH, PYTHONDONTWRITEBYTECODE, PYTHONWARNINGS, python -E, sys.flags.ignore_environment, PYTHON environment variables]
area: languages/python/tooling
level: core
summary: >-
  Source files are UTF-8 unless a coding comment on line 1 or 2 names another encoding. PYTHON* environment variables set interpreter options, command-line switches win, and -E or -I ignore them.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/lexical_analysis.html", locator: "2.1.4. Encoding declarations"}
see_also: [python/the-python-interpreter, python/invoking-the-interpreter, python/development-mode-and-x-options, python/compiled-python-files, python/environment-variables-from-python]
sources:
  - url: https://docs.python.org/3.15/tutorial/interpreter.html
    title: Using the Python Interpreter, The Python Tutorial 3.15
    locator: 2.2.1. Source Code Encoding
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html
    title: Lexical analysis, Python 3.15 documentation
    locator: 2.1.4. Encoding declarations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/tokenize.html
    title: tokenize, Python 3.15 documentation
    locator: detect_encoding
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/cmdline.html
    title: Command line and environment, Python 3.15 documentation
    locator: 1.2. Environment variables; -E; -I
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html
    title: sys, Python 3.15 documentation
    locator: sys.dont_write_bytecode; sys.flags
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Source code representation
    accessed: 2026-11-16
  - url: https://pkg.go.dev/cmd/go
    title: Command go, Go documentation
    locator: Print Go environment information; Environment variables
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import os
import pathlib
import subprocess
import sys
import tempfile

body = "print('café', len('café'))\n"
with tempfile.TemporaryDirectory() as tmp:
    for name, text in [('declared.py', '# -*- coding: cp1252 -*-\n' + body),
                       ('undeclared.py', body)]:
        path = pathlib.Path(tmp, name)
        path.write_bytes(text.encode('cp1252'))
        result = subprocess.run([sys.executable, name], cwd=tmp,
                                capture_output=True, text=True)
        out = result.stdout or result.stderr.splitlines()[-1]
        print(name, result.returncode, out.replace(os.path.realpath(tmp), '<tmp>').strip())
```

```output for=synopsis
declared.py 0 café 4
undeclared.py 1 SyntaxError: Non-UTF-8 code starting with '\xe9' in file <tmp>/undeclared.py on line 1, but no encoding declared; see https://peps.python.org/pep-0263/ for details
```

Both files hold the same bytes for the `print` line, written in Windows-1252, where `é` is the single byte `0xe9`. The first file says so on its first line and runs. The second has no declaration, so the interpreter reads it as UTF-8, where `0xe9` alone is not a valid character, and refuses to run it.

## Semantics

**r1.** "By default, Python source files are treated as encoded in UTF-8."[^1] The reference says the same: "If no encoding declaration is found, the default encoding is UTF-8."[^2]

**r2.** "To declare an encoding other than the default one, a special comment line should be added as the first line of the file."[^1] The tutorial gives the form `# -*- coding: cp1252 -*-`.[^1] "One exception to the first line rule is when the source code starts with a UNIX “shebang” line. In this case, the encoding declaration should be added as the second line of the file."[^1]

**r3.** The reference gives the exact rule. "If a comment in the first or second line of the Python script matches the regular expression coding[=:]\s*([-\w.]+), this comment is processed as an encoding declaration".[^2] "If it is the second line, the first line must also be a comment-only line."[^2] Besides the Emacs form, `# vim:fileencoding=<encoding-name>` is a recommended form.[^2] "If an encoding is declared, the encoding name must be recognized by Python".[^2]

**r4.** The encoding applies to the whole file: "The encoding is used for all lexical analysis, including string literals, comments and identifiers."[^2] A UTF-8 byte-order mark at the start "is ignored rather than being a syntax error" when the file is UTF-8.[^2]

**r5.** `tokenize.detect_encoding(readline)` finds the encoding the way the interpreter does. "It will call readline a maximum of twice, and return the encoding used (as a string) and a list of any lines (not decoded from bytes) it has read in."[^3] "Note that if the BOM is found, 'utf-8-sig' will be returned as an encoding. If no encoding is specified, then the default of 'utf-8' will be returned."[^3]

**r6.** Environment variables named `PYTHON...` set interpreter options. "These environment variables influence Python’s behavior, they are processed before the command-line switches other than -E or -I. It is customary that command-line switches override environmental variables where there is a conflict."[^4]

**r7.** `PYTHONPATH` is one of them: it is meant to "Augment the default search path for module files", with directories separated by `os.pathsep`.[^4] `PYTHONDONTWRITEBYTECODE` set to a non-empty string means "Python won’t try to write .pyc files on the import of source modules. This is equivalent to specifying the -B option."[^4] Inside the program, `sys.dont_write_bytecode` "is initially set to True or False depending on the -B command line option and the PYTHONDONTWRITEBYTECODE environment variable".[^5] `PYTHONWARNINGS` "is equivalent to the -W option."[^4]

**r8.** `-E` means "Ignore all PYTHON* environment variables, e.g. PYTHONPATH and PYTHONHOME, that might be set."[^4] `-I` runs "in isolated mode", which "also implies -E, -P and -s options."[^4] A program sees these options in `sys.flags`, whose `ignore_environment` field reflects `-E` and whose `isolated` field reflects `-I`.[^5]

## Examples

### Where a declaration counts

```python run id=placement
import io
import tokenize

sources = {
    'line 1': b"# -*- coding: cp1252 -*-\ns = 'caf\xe9'\n",
    'after shebang': b"#!/usr/bin/env python3\n# vim:fileencoding=cp1252\ns = 'caf\xe9'\n",
    'line 3': b"\n\n# -*- coding: cp1252 -*-\ns = 'caf\xe9'\n",
    'BOM': b"\xef\xbb\xbfs = 'caf\xc3\xa9'\n",
}
for label, src in sources.items():
    encoding, lines = tokenize.detect_encoding(io.BytesIO(src).readline)
    try:
        namespace = {}
        exec(compile(src, label, 'exec'), namespace)
        result = namespace['s']
    except SyntaxError as e:
        result = 'SyntaxError'
    print(f'{label:14} {encoding:10} {len(lines)} {result}')
```

```output for=placement
line 1         cp1252     1 café
after shebang  cp1252     2 café
line 3         utf-8      2 SyntaxError
BOM            utf-8-sig  1 café
```

A declaration on line 1, or on line 2 after a shebang, is found; the `vim:` form works too (r3). On line 3 it is just a comment: `detect_encoding` stops after two lines and returns the default, and the byte `0xe9` makes the source invalid UTF-8 (r5). A file that starts with a UTF-8 byte-order mark needs no declaration (r4). The middle column is the number of lines `detect_encoding` read.

### PYTHON* variables, -E and -I

```python run id=environment
import os
import pathlib
import subprocess
import sys
import tempfile

code = ('import sys; print(sys.argv[1] in sys.path, sys.dont_write_bytecode,'
        ' sys.flags.ignore_environment, sys.flags.isolated)')
with tempfile.TemporaryDirectory() as extra:
    env = dict(os.environ, PYTHONPATH=extra, PYTHONDONTWRITEBYTECODE='1')
    for opts in ([], ['-E'], ['-I']):
        result = subprocess.run([sys.executable, *opts, '-c', code, extra], env=env,
                                capture_output=True, text=True)
        print(f'{str(opts):7}', result.stdout.strip())
```

```output for=environment
[]      True True 0 0
['-E']  False False 1 0
['-I']  False False 1 1
```

The child gets `PYTHONPATH` and `PYTHONDONTWRITEBYTECODE` from its environment. Without options, the extra directory is on `sys.path` and bytecode writing is off (r7). With `-E` both variables are ignored and `sys.flags.ignore_environment` is 1; `-I` does the same and also sets `sys.flags.isolated` (r8).

### A command-line switch beats a variable

```python run id=precedence
import os
import subprocess
import sys

env = dict(os.environ, PYTHONWARNINGS='error')
code = 'import warnings; warnings.warn("old API"); print("went on")'
for opts in ([], ['-W', 'default'], ['-E']):
    result = subprocess.run([sys.executable, *opts, '-c', code], env=env,
                            capture_output=True, text=True)
    print(f'{str(opts):17}', result.returncode, (result.stdout or result.stderr.splitlines()[-1]).strip())
```

```output for=precedence
[]                1 UserWarning: old API
['-W', 'default'] 0 went on
['-E']            0 went on
```

`PYTHONWARNINGS=error` turns the warning into an exception, so the program stops. `-W default` on the command line wins over the variable (r6), and the warning is only printed to stderr. `-E` drops the variable altogether (r8).

## Pitfalls

- An editor that saves a file in a legacy encoding without a declaration produces a file the interpreter refuses (see the synopsis). Save source as UTF-8 and leave the declaration out.
- A declaration below line 2, or on line 2 after a line that is not a comment, is ignored (r3).
- The declaration names the file's real encoding; it does not convert anything. If the file was saved as UTF-8 but declares `cp1252`, the text is decoded as cp1252 and non-ASCII characters come out wrong.
- A `PYTHONPATH` or `PYTHONWARNINGS` set in a shell profile changes every program run from that shell. When a program behaves differently on one machine, compare the `PYTHON...` variables, or run it with `-E` or `-I` (r8).
- `-I` ignores the environment and also drops the script directory and the user site directory from `sys.path` (r8), so a script that relied on either cannot import those modules.

## Comparison

Go has no encoding declaration: "Source code is Unicode text encoded in UTF-8."[^6] The go command, like the interpreter, reads its options from the environment: "The go command and the tools it invokes consult environment variables for configuration."[^7] It also has a command for them: "To see the effective setting of the variable <NAME>, run 'go env <NAME>'."[^7] `GOFLAGS` plays the part of a variable that sets command-line options, and "Flags listed on the command line are applied after this list and therefore override it", the same order as Python (r6).[^7]

[^1]: Using the Python Interpreter, The Python Tutorial 3.15.
[^2]: Lexical analysis, Python 3.15 documentation.
[^3]: tokenize, Python 3.15 documentation.
[^4]: Command line and environment, Python 3.15 documentation.
[^5]: sys, Python 3.15 documentation.
[^6]: The Go Programming Language Specification.
[^7]: Command go, Go documentation.
