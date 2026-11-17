---
tag: MZM9J0
kind: feature
owner: python
slug: the-customization-modules
name: The Customization Modules
aliases: [sitecustomize, usercustomize, site module, user site-packages, site.ENABLE_USER_SITE, PYTHONNOUSERSITE, PYTHONUSERBASE, "python -s", "python -S"]
area: languages/python/tooling
level: advanced
summary: >-
  sitecustomize and usercustomize are modules that the site module imports at every start of Python. What each is for, how -S, -s, -I and virtual environments turn them off, and how errors show.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/site.html", locator: "sitecustomize; usercustomize"}
see_also: [python/the-interactive-startup-file, python/the-module-search-path, python/the-interpreter-and-its-environment, python/invoking-the-interpreter, python/tab-completion-and-history-editing, python/startup-time]
sources:
  - url: https://docs.python.org/3.15/library/site.html
    title: site, Python 3.15 documentation
    locator: module overview; sitecustomize; usercustomize; Module contents; Command-line interface
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/using/cmdline.html
    title: Command line and environment, Python 3.15 documentation
    locator: -s; -S; -I; PYTHONNOUSERSITE; PYTHONUSERBASE
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/tutorial/appendix.html
    title: Appendix, The Python Tutorial 3.15
    locator: 16.1.4. The Customization Modules
    accessed: 2026-11-18
  - url: https://pkg.go.dev/cmd/go
    title: Command go, Go documentation
    locator: Environment variables (GOFLAGS, GOENV); Print Go environment information
    accessed: 2026-11-18
---

## Synopsis

```python run id=synopsis
import os
import pathlib
import subprocess
import sys
import tempfile

with tempfile.TemporaryDirectory() as tmp:
    sitedir = pathlib.Path(tmp, 'site')
    sitedir.mkdir()
    (sitedir / 'sitecustomize.py').write_text("import sys\nsys.site_hook = 'sitecustomize'\n")
    env = dict(os.environ, PYTHONPATH=str(sitedir))
    code = "import sys; print(getattr(sys, 'site_hook', None))"
    for opts in ([], ['-S'], ['-I']):
        result = subprocess.run([sys.executable, *opts, '-c', code], env=env,
                                capture_output=True, text=True)
        print(opts, result.stdout.strip())
```

```output for=synopsis
[] sitecustomize
['-S'] None
['-I'] None
```

The program puts a `sitecustomize.py` on `PYTHONPATH`; it sets an attribute on `sys`. A plain start imports it before running the `-c` code, which can see the attribute. With `-S`, `site` is not imported, so neither is `sitecustomize` (r1). `-I` keeps `site`, but it ignores `PYTHONPATH` (r5), so `sitecustomize` is not found.

## Semantics

**r1.** The `site` module runs at start-up. "This module is automatically imported during initialization. The automatic import can be suppressed using the interpreter’s -S option."[^1] `-S` means "Disable the import of the module site and the site-dependent manipulations of sys.path that it entails."[^2]

**r2.** "Python provides two hooks to let you customize it: sitecustomize and usercustomize."[^3] `usercustomize` "will affect every invocation of Python, unless it is started with the -s option to disable the automatic import. sitecustomize works in the same way, but is typically created by an administrator of the computer in the global site-packages directory, and is imported before usercustomize."[^3]

**r3.** `sitecustomize`: "After these path manipulations, an attempt is made to import a module named sitecustomize , which can perform arbitrary site-specific customizations."[^1] "If this import fails with an ImportError or its subclass exception, and the exception’s name attribute equals 'sitecustomize' , it is silently ignored."[^1] So a missing `sitecustomize` is normal. The docs warn about the rest: "Any other exception causes a silent and perhaps mysterious failure of the process."[^1]

**r4.** `usercustomize` is imported next, "if ENABLE_USER_SITE is true. This file is intended to be created in the user site-packages directory (see below), which is part of sys.path unless disabled by -s ."[^1] Again, if the import "fails with an ImportError or its subclass exception, and the exception’s name attribute equals 'usercustomize' , it is silently ignored."[^1]

**r5.** `site.ENABLE_USER_SITE` tells whether the user directory is on. "True means that it is enabled and was added to sys.path . False means that it was disabled by user request (with -s or PYTHONNOUSERSITE ). None means it was disabled for security reasons (mismatch between user or group id and effective id) or by an administrator."[^1] `-I` "also implies -E , -P and -s options", and `-E` ignores all `PYTHON*` variables, `PYTHONPATH` among them.[^2]

**r6.** Virtual environments change the user directory too. In a virtual environment, if `include-system-site-packages` is not true, the system prefixes are not searched, and "If the system-level prefixes are not searched then the user site prefixes are also implicitly not searched for site-packages."[^1]

**r7.** `PYTHONUSERBASE` "Defines the user base directory , which is used to compute the path of the user site-packages directory".[^2] `python -m site --user-site` prints that directory, and the tutorial's way to find it is `site.getusersitepackages()`.[^1][^3]

## Examples

### What happens when sitecustomize fails

```python run id=errors
import os
import pathlib
import subprocess
import sys
import tempfile

hooks = {
    'none': None,
    'ok': "import sys\nsys.site_hook = 'sitecustomize ran'\n",
    'missing dep': "import no_such_module\n",
    'error': "raise ValueError('bad setting')\n",
}
code = "import sys; print(getattr(sys, 'site_hook', None))"
with tempfile.TemporaryDirectory() as tmp:
    for name, source in hooks.items():
        sitedir = pathlib.Path(tmp, name.replace(' ', '-'))
        sitedir.mkdir()
        if source is not None:
            (sitedir / 'sitecustomize.py').write_text(source)
        env = dict(os.environ, PYTHONPATH=str(sitedir))
        result = subprocess.run([sys.executable, '-c', code], env=env,
                                capture_output=True, text=True)
        print(f'{name:12} stdout={result.stdout.strip()!r} stderr={result.stderr.strip()!r}')
```

```output for=errors
none         stdout='None' stderr=''
ok           stdout='sitecustomize ran' stderr=''
missing dep  stdout='None' stderr="Error in sitecustomize; set PYTHONVERBOSE for traceback:\nModuleNotFoundError: No module named 'no_such_module'"
error        stdout='None' stderr='Error in sitecustomize; set PYTHONVERBOSE for traceback:\nValueError: bad setting'
```

With no `sitecustomize` at all, nothing is printed (r3). A `sitecustomize` that imports a missing module also raises an `ImportError`, but its `name` is `'no_such_module'`, not `'sitecustomize'`, so it is not ignored. On the interpreter that ran this, both failures print a two-line message on standard error, and the program still runs. A start-up that "works" while printing this message has lost its customization.

### The user site and virtual environments

```python run id=user-site
import os
import pathlib
import subprocess
import tempfile
import venv

with tempfile.TemporaryDirectory() as tmp:
    env = dict(os.environ, PYTHONUSERBASE=os.path.join(tmp, 'userbase'))
    code = "import site; print('ENABLE_USER_SITE =', site.ENABLE_USER_SITE)"
    for name, system in [('shared', True), ('isolated', False)]:
        venv.create(os.path.join(tmp, name), system_site_packages=system)
        python = os.path.join(tmp, name, 'bin', 'python')
        if system:
            user_site = subprocess.run([python, '-m', 'site', '--user-site'], env=env,
                                       capture_output=True, text=True).stdout.strip()
            pathlib.Path(user_site).mkdir(parents=True)
            pathlib.Path(user_site, 'usercustomize.py').write_text("print('usercustomize ran')\n")
        for opts in ([], ['-s']):
            result = subprocess.run([python, *opts, '-c', code], env=env,
                                    capture_output=True, text=True)
            print(f'{name:8} {str(opts):6}', result.stdout.strip().replace('\n', ' / '))
```

```output for=user-site
shared   []     usercustomize ran / ENABLE_USER_SITE = True
shared   ['-s'] ENABLE_USER_SITE = False
isolated []     ENABLE_USER_SITE = False
isolated ['-s'] ENABLE_USER_SITE = False
```

`PYTHONUSERBASE` moves the user directory into the temporary directory (r7), and a `usercustomize.py` goes in it. The `shared` environment was made with `system_site_packages=True`, so the user directory is on and `usercustomize` runs before the `-c` code (r4). `-s` turns it off. The `isolated` environment does not search the system prefixes, so it does not search the user directory either, with or without `-s` (r6). The example uses POSIX paths (`bin/python`).

## Pitfalls

- `sitecustomize` and `usercustomize` run at every start of that interpreter, for every program (r2). Slow or failing code there slows or breaks all of them.
- A failure inside `sitecustomize` does not stop the program (see the errors example). Watch standard error for "Error in sitecustomize" after changing it.
- A `usercustomize` that works in a normal shell is skipped in most virtual environments (r6) and under `-s` or `-I` (r5).
- These modules are found on `sys.path` like any other module. A `sitecustomize.py` earlier on the path, for example from `PYTHONPATH`, replaces the one in site-packages (see the synopsis).
- For per-session settings at the prompt, the interactive startup file is the narrower tool: it runs only for interactive sessions.

## Comparison

The go command's nearest feature takes settings, not code: `GOFLAGS` is "A space-separated list of -flag=value settings to apply to go commands by default", and "Flags listed on the command line are applied after this list and therefore override it."[^4] Per-user defaults go in a configuration file through `go env -w`, which "changes the default settings of the named environment variables to the given values".[^4] That covers the settings part of `usercustomize`, without running code.

[^3]: Appendix, The Python Tutorial 3.15.
[^1]: site, Python 3.15 documentation.
[^2]: Command line and environment, Python 3.15 documentation.
[^4]: Command go, Go documentation.
