---
tag: NEW-python-virtual-environments
kind: feature
owner: python
slug: virtual-environments
name: Creating Virtual Environments
aliases: [python -m venv, venv module, activate script, deactivate, pyvenv.cfg, sys.prefix, sys.base_prefix, VIRTUAL_ENV, EnvBuilder, uv venv, .venv]
area: languages/python/modules-and-packages
level: intro
summary: >-
  python -m venv dir makes a directory with pyvenv.cfg, a bin directory and its own site-packages. Its python sets sys.prefix to the directory. Activation only changes PATH, and it is optional.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/venv.html", locator: "Creating virtual environments; How venvs work"}
see_also: [python/virtual-environments-and-packages, python/managing-packages-with-pip, python/the-module-search-path, python/standard-modules]
sources:
- url: https://docs.python.org/3.15/library/venv.html
  title: venv — Creation of virtual environments
  locator: Creating virtual environments; How venvs work; API
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/tutorial/venv.html
  title: The Python Tutorial, 12. Virtual Environments and Packages
  locator: 12.2. Creating Virtual Environments
  accessed: 2026-11-18
- url: https://packaging.python.org/en/latest/specifications/virtual-environments/
  title: Python Virtual Environments, Python Packaging User Guide
  locator: Runtime detection of virtual environments; Declaring installation environments as Python virtual environments
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/library/sys.html#sys.base_prefix
  title: sys — System-specific parameters and functions
  locator: sys.base_prefix
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/library/site.html
  title: site — Site-specific configuration hook
  locator: site; Path extensions (.pth files)
  accessed: 2026-11-18
- url: https://docs.astral.sh/uv/pip/environments/
  title: Using Python environments, uv documentation
  locator: Using Python environments; Creating a virtual environment; Using arbitrary Python environments
  accessed: 2026-11-18
- url: https://go.dev/ref/mod#module-cache
  title: Go Modules Reference
  locator: Module cache; go.mod files; require directive
  accessed: 2026-11-18
---

## Synopsis

```text
python -m venv .venv                   # create; --without-pip, --system-site-packages, --clear, --prompt NAME
source .venv/bin/activate              # bash and zsh; activate.fish, activate.csh, Activate.ps1
.venv\Scripts\activate                 # Windows
deactivate

.venv/bin/python script.py             # no activation needed
.venv/
    pyvenv.cfg                         # home = <base Python>, include-system-site-packages = false
    bin/                               # python, python3, activate scripts, installed scripts
    lib/pythonX.Y/site-packages/       # the installed packages

venv.create(".venv", with_pip=True)    # the same from Python; venv.EnvBuilder for hooks
```

## Semantics

**r1.** "Virtual environments are created by executing the venv module: python -m venv /path/to/new/virtual/environment This creates the target directory (including parent directories as needed) and places a pyvenv.cfg file in it with a home key pointing to the Python installation from which the command was run. It also creates a bin (or Scripts on Windows) subdirectory containing a copy or symlink of the Python executable (as appropriate for the platform or arguments used at environment creation time). It also creates a lib/pythonX.Y/site-packages subdirectory (on Windows, this is Lib\site-packages)."[^1] Example: [`layout`](#layout).

**r2.** The environment has the Python version that made it. The tutorial says: "venv will install the Python version from which the command was run (as reported by the --version option). For instance, executing the command with python3.12 will install version 3.12."[^2] The tutorial names the usual directory: "A common directory location for a virtual environment is .venv."[^2]

**r3.** "When a Python interpreter is running from a virtual environment, sys.prefix and sys.exec_prefix point to the directories of the virtual environment, whereas sys.base_prefix and sys.base_exec_prefix point to those of the base Python used to create the environment. It is sufficient to check sys.prefix != sys.base_prefix to determine if the current interpreter is running from a virtual environment."[^1] The sys page says that `base_prefix` "does not change, and always points to the base Python installation."[^4] Example: [`prefix`](#prefix).

**r4.** The `site` module reads the file at startup: "When running under a virtual environment, the pyvenv.cfg file in sys.prefix is checked for site-specific configurations. If the include-system-site-packages key exists and is set to true (case-insensitive), the system-level prefixes will be searched for site-packages, otherwise they won’t."[^5] The created file "includes the include-system-site-packages key, set to true if venv is run with the --system-site-packages option, false otherwise."[^1] Examples: [`layout`](#layout), [`prefix`](#prefix).

**r5.** Activation changes the shell, not Python. The activate script "will prepend that directory to your PATH, so that running python will invoke the environment’s Python interpreter and you can run installed scripts without having to use their full path."[^1] "When a virtual environment has been activated, the VIRTUAL_ENV environment variable is set to the path of the environment."[^1] Example: [`activate`](#activate).

**r6.** Activation is optional. "You don’t specifically need to activate a virtual environment, as you can just specify the full path to that environment’s Python interpreter when invoking Python. Furthermore, all scripts installed in the environment should be runnable without activating it."[^1] Examples: [`prefix`](#prefix), [`virtual-env-check`](#virtual-env-check).

**r7.** By default the command also installs pip: "Unless the --without-pip option is given, ensurepip will be invoked to bootstrap pip into the virtual environment."[^1] The function `venv.create()` has `with_pip=False` as its default.[^1] Example: [`layout`](#layout).

**r8.** The class `venv.EnvBuilder` makes environments, and a subclass can change the steps. "Each of the methods ensure_directories(), create_configuration(), setup_python(), setup_scripts() and post_setup() can be overridden."[^1] `post_setup()` is "A placeholder method which can be overridden in third party implementations to pre-install packages in the virtual environment or perform other post-creation steps."[^1] Example: [`builder`](#builder).

## Examples

The paths in the examples are for POSIX. On Windows the directory is `Scripts`, not `bin`.

`venv.create()` makes the directories and `pyvenv.cfg` (rule r1). The function does not install pip unless `with_pip=True` is given (rule r7).

```python run id=layout
import os
import sys
import venv

venv.create("env", symlinks=True)

version = f"python{sys.version_info.major}.{sys.version_info.minor}"
print(os.path.isfile("env/pyvenv.cfg"), os.path.isdir(f"env/lib/{version}/site-packages"))
names = os.listdir("env/bin")
print(sorted(n for n in names if n.startswith(("activate", "Activate"))))
print("python" in names, "python3" in names, version in names)
with open("env/pyvenv.cfg") as f:
    config = dict(line.rstrip("\n").split(" = ", 1) for line in f)
print(config["include-system-site-packages"], "home" in config)
```

```output for=layout
True True
['Activate.ps1', 'activate', 'activate.csh', 'activate.fish']
True True True
false True
```

The Python of the environment knows its environment from `sys.prefix` (rule r3). Its `sys.path` has the site-packages directory of the environment (rule r4). The program runs it by its full path, with no activation (rule r6).

```python run id=prefix
import json
import os
import subprocess
import sys
import venv

venv.create("env", symlinks=True)

code = "import json, sys; print(json.dumps([sys.prefix, sys.base_prefix, sys.path]))"
out = subprocess.run(["env/bin/python", "-c", code], capture_output=True, text=True).stdout
prefix, base_prefix, path = json.loads(out)

print(prefix == os.path.abspath("env"))
print(base_prefix == sys.base_prefix, prefix != base_prefix)
version = f"python{sys.version_info.major}.{sys.version_info.minor}"
print(os.path.join(prefix, "lib", version, "site-packages") in path)
```

```output for=prefix
True
True True
True
```

The activate script puts `env/bin` first in `PATH` and sets `VIRTUAL_ENV` (rule r5). `deactivate` undoes both.

```python run id=activate
import os
import subprocess
import venv

venv.create("env", symlinks=True)

script = """
source env/bin/activate
python -c 'import sys; print(sys.prefix != sys.base_prefix)'
basename "$VIRTUAL_ENV"
[ "$(command -v python)" = "$VIRTUAL_ENV/bin/python" ] && echo "python is env/bin/python"
deactivate
echo "VIRTUAL_ENV=${VIRTUAL_ENV:-unset}"
"""
clean = {k: v for k, v in os.environ.items() if k != "VIRTUAL_ENV"}
subprocess.run(["bash", "-c", script], env=clean, check=True)
```

```output for=activate
True
env
python is env/bin/python
VIRTUAL_ENV=unset
```

A subclass of `EnvBuilder` adds a step after the creation (rule r8). This one writes a `.pth` file, so the environment can import modules from a shared directory. The `site` page says that a `.pth` file "contains additional items (one per line) to be appended to sys.path."[^5]

```python norun reason=fragment file=shared/units.py group=builder
METRE = 1.0
```

```python run file=main.py group=builder id=builder
import os
import subprocess
import venv


class SharedEnvBuilder(venv.EnvBuilder):
    def post_setup(self, context):
        print("post_setup:", context.env_name, context.bin_name)
        with open(os.path.join(context.lib_path, "shared.pth"), "w") as f:
            f.write(os.path.abspath("shared") + "\n")


SharedEnvBuilder(symlinks=True).create("env")
code = "import units; print(units.METRE)"
print(subprocess.run(["env/bin/python", "-c", code], capture_output=True, text=True).stdout, end="")
```

```output for=builder
post_setup: env bin
1.0
```

The examples give the same output on CPython 3.12.3, 3.13.14, 3.14.6 and 3.15.0b2.

## Versions

The venv page lists two changes to the command: "Changed in version 3.12: setuptools is no longer a core venv dependency." and "Changed in version 3.13: venv now creates a .gitignore file for Git by default."[^1] The function `venv.create()` writes no `.gitignore` unless it gets `scm_ignore_files={"git"}`.[^1]

## Pitfalls

### A test of VIRTUAL_ENV

`VIRTUAL_ENV` is set only by the activate script (rule r5). A Python that runs from the environment by its full path does not have it (rule r6). The venv page says: "Since explicitly activating a virtual environment is not required to use it, VIRTUAL_ENV cannot be relied upon to determine whether a virtual environment is being used."[^1]

```python wrong id=virtual-env-check
import os
import subprocess
import venv

venv.create("env", symlinks=True)

code = "import os, sys; print('VIRTUAL_ENV' in os.environ, sys.prefix != sys.base_prefix)"
clean = {k: v for k, v in os.environ.items() if k != "VIRTUAL_ENV"}
print(subprocess.run(["env/bin/python", "-c", code], env=clean, capture_output=True, text=True).stdout, end="")
```

```output for=virtual-env-check
False True
```

Test `sys.prefix != sys.base_prefix` (rule r3).

### A moved environment

An installed script has the absolute path of the environment in its first line. The venv page says: "Because scripts installed in environments should not expect the environment to be activated, their shebang lines contain the absolute paths to their environment’s interpreters. Because of this, environments are inherently non-portable, in the general case."[^1] After a move, the script refers to a Python that does not exist.

```python run file=main.py group=moved id=moved
import os
import subprocess
import venv

venv.create("env", symlinks=True)
script = "env/bin/hello"
with open(script, "w") as f:
    f.write("#!" + os.path.abspath("env/bin/python") + "\nprint('hello')\n")
os.chmod(script, 0o755)

print(subprocess.run([script], capture_output=True, text=True).stdout, end="")
os.rename("env", "moved")
try:
    subprocess.run(["moved/bin/hello"])
except FileNotFoundError as e:
    print("FileNotFoundError:", e.strerror)
```

```output for=moved
hello
FileNotFoundError: No such file or directory
```

The script in the example is like the scripts that pip writes. The venv page gives the fix: "If for any reason you need to move the environment to a new location, you should recreate it at the desired location and delete the one at the old location."[^1]

## Comparison

**Go** has no environments. It keeps downloaded modules in one cache: "The module cache is the directory where the go command stores downloaded module files."[^7] "The cache may be shared by multiple Go projects developed on the same machine."[^7] A Go project lists its versions in its `go.mod` file: "A require directive declares a minimum required version of a given module dependency."[^7] A Python environment holds one version of each package, so each Python project needs its own environment (see [[python/virtual-environments-and-packages]]).

## Implementation notes

The packaging specification gives the smallest form: "a Python virtual environment in its simplest form consists of nothing more than a copy or symlink of the Python binary accompanied by a site-packages directory and a pyvenv.cfg file with a home key that indicates where to find the Python standard library modules."[^3] Other tools make environments of this form. The uv documentation says: "A virtual environment is a lightweight way to isolate packages from a Python installation's environment. Unlike pip, uv requires using a virtual environment by default."[^6] The command `uv venv` makes an environment in `.venv`.[^6] uv also uses an environment that it did not make: "setting VIRTUAL_ENV=/path/to/venv will cause uv to install into /path/to/venv, regardless of where uv is installed."[^6]

## References
