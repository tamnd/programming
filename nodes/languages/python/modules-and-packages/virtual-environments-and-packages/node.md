---
tag: NEW-python-virtual-environments-and-packages
kind: feature
owner: python
slug: virtual-environments-and-packages
name: Virtual Environments and Packages
aliases: [virtual environment, venv, third-party packages, project environment, requirements.txt, conflicting requirements, PyPI]
area: languages/python/modules-and-packages
level: intro
summary: >-
  Each project gets its own virtual environment, a directory with a Python and its own installed packages. pip installs into it, and pip freeze records its packages so that you can create it again.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/venv.html", locator: "12. Virtual Environments and Packages"}
see_also: [python/virtual-environments, python/managing-packages-with-pip, python/the-module-search-path, python/standard-modules, python/modules]
sources:
- url: https://docs.python.org/3.15/tutorial/venv.html
  title: The Python Tutorial, 12. Virtual Environments and Packages
  locator: 12.1. Introduction; 12.2. Creating Virtual Environments; 12.3. Managing Packages with pip
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/library/venv.html
  title: venv — Creation of virtual environments
  locator: venv — Creation of virtual environments; How venvs work
  accessed: 2026-11-18
- url: https://packaging.python.org/en/latest/guides/installing-using-pip-and-virtual-environments/
  title: Install packages in a virtual environment using pip and venv, Python Packaging User Guide
  locator: Create a new virtual environment; Install from local archives; Freezing dependencies
  accessed: 2026-11-18
- url: https://docs.npmjs.com/cli/v11/commands/npm-install
  title: npm-install, npm Docs
  locator: Description
  accessed: 2026-11-18
- url: https://pip.pypa.io/en/stable/cli/pip_freeze/
  title: pip freeze, pip documentation
  locator: Options
  accessed: 2026-11-18
---

## Synopsis

```text
python -m venv .venv                       # one environment for one project
source .venv/bin/activate                  # POSIX shells; .venv\Scripts\activate on Windows
python -m pip install requests==2.6.0      # installs into .venv only
python -m pip freeze > requirements.txt    # record the installed versions
deactivate

python -m venv .venv                       # later, or on another machine:
.venv/bin/python -m pip install -r requirements.txt
```

## Semantics

**r1.** One Python installation can have only one version of each package. The tutorial says: "This means it may not be possible for one Python installation to meet the requirements of every application. If application A needs version 1.0 of a particular module but application B needs version 2.0, then the requirements are in conflict and installing either version 1.0 or 2.0 will leave one application unable to run."[^1] Example: [`two-apps`](#two-apps).

**r2.** "The solution for this problem is to create a virtual environment, a self-contained directory tree that contains a Python installation for a particular version of Python, plus a number of additional packages. Different applications can then use different virtual environments."[^1] Example: [`two-apps`](#two-apps).

**r3.** An environment starts empty. It is "created on top of an existing Python installation, known as the virtual environment’s “base” Python, and by default is isolated from the packages in the base environment, so that only those explicitly installed in the virtual environment are available."[^2] Example: [`two-apps`](#two-apps).

**r4.** The tools find the environment. "When used from within a virtual environment, common installation tools such as pip will install Python packages into a virtual environment without needing to be told to do so explicitly."[^2] "By default pip will install packages from the Python Package Index."[^1] Example: [`two-apps`](#two-apps).

**r5.** An environment is not part of the source code. The venv page says that an environment is "Not checked into source control systems such as Git", "Considered as disposable – it should be simple to delete and recreate it from scratch", and "Not considered as movable or copyable – you just recreate the same environment in the target location."[^2] Example: [`recreate`](#recreate).

**r6.** A requirements file records the packages. "python -m pip freeze will produce a similar list of the installed packages, but the output uses the format that python -m pip install expects. A common convention is to put this list in a requirements.txt file"[^1] "The requirements.txt can then be committed to version control and shipped as part of an application."[^1] Example: [`freeze`](#freeze).

**r7.** The tutorial chapter has three parts. The page [[python/virtual-environments]] tells how to create and activate an environment. The page [[python/managing-packages-with-pip]] tells how to install, list and remove packages.

## Examples

The examples do not use the network. A small helper module writes wheel files, which are the files that pip installs, into a local directory. The option `--no-index --find-links=<dir>` tells pip to use only that directory, and the packaging guide says that this is "useful if you are installing packages on a system with limited connectivity or if you want to strictly control the origin of distribution packages."[^3] The page [[python/build-backends-and-wheels]] explains the wheel format.

Each example runs the pip of the outer Python with `--python <env>/bin/python`, so the environments are created without pip and quickly. In a shell you activate the environment and run `python -m pip`. The paths are for POSIX. On Windows the directory is `Scripts`, not `bin`.

Two applications need two versions of `greet` (rule r1). Each one gets its own environment (rule r2). The outer Python does not see either package (rule r3). The two installs run at the same time, because they do not share files.

```python norun reason=fragment file=mkwheel.py group=two-apps
import base64
import hashlib
import zipfile


def make_wheel(folder, name, version, code, requires=()):
    info = f"{name}-{version}.dist-info"
    meta = f"Metadata-Version: 2.1\nName: {name}\nVersion: {version}\n"
    meta += "".join(f"Requires-Dist: {r}\n" for r in requires)
    files = {
        f"{name}.py": code,
        f"{info}/METADATA": meta,
        f"{info}/WHEEL": "Wheel-Version: 1.0\nRoot-Is-Purelib: true\nTag: py3-none-any\n",
    }
    record = []
    for path, text in files.items():
        digest = base64.urlsafe_b64encode(hashlib.sha256(text.encode()).digest())
        record.append(f"{path},sha256={digest.rstrip(b'=').decode()},{len(text.encode())}\n")
    files[f"{info}/RECORD"] = "".join(record) + f"{info}/RECORD,,\n"
    with zipfile.ZipFile(f"{folder}/{name}-{version}-py3-none-any.whl", "w") as z:
        for path, text in files.items():
            z.writestr(path, text)
```

```python run file=main.py group=two-apps id=two-apps
import os
import subprocess
import sys
import venv

from mkwheel import make_wheel

os.mkdir("wheels")
make_wheel("wheels", "greet", "1.0", 'VERSION = "1.0"\n')
make_wheel("wheels", "greet", "2.0", 'VERSION = "2.0"\n')

installs = []
for env, want in [("app-a", "1.0"), ("app-b", "2.0")]:
    venv.create(env)
    installs.append(subprocess.Popen(
        [sys.executable, "-m", "pip", "--python", f"{env}/bin/python", "install",
         "--no-index", "--find-links", "wheels", f"greet=={want}"],
        stdout=subprocess.DEVNULL,
    ))
assert all(p.wait() == 0 for p in installs)

code = "import greet; print(greet.VERSION)"
for env in ["app-a", "app-b"]:
    out = subprocess.run([f"{env}/bin/python", "-c", code], capture_output=True, text=True)
    print(env, out.stdout.strip())

outer = subprocess.run([sys.executable, "-c", code], capture_output=True, text=True)
print("outer Python:", outer.stderr.strip().splitlines()[-1])
```

```output for=two-apps
app-a 1.0
app-b 2.0
outer Python: ModuleNotFoundError: No module named 'greet'
```

`pip freeze` writes the pinned versions (rule r6). `greet` 2.0 depends on `colors`, so the list has both packages. The outer pip reads the environment with the option `--path`, which will "Restrict to the specified installation path for listing packages".[^5]

```python norun reason=fragment file=mkwheel.py group=freeze
import base64
import hashlib
import zipfile


def make_wheel(folder, name, version, code, requires=()):
    info = f"{name}-{version}.dist-info"
    meta = f"Metadata-Version: 2.1\nName: {name}\nVersion: {version}\n"
    meta += "".join(f"Requires-Dist: {r}\n" for r in requires)
    files = {
        f"{name}.py": code,
        f"{info}/METADATA": meta,
        f"{info}/WHEEL": "Wheel-Version: 1.0\nRoot-Is-Purelib: true\nTag: py3-none-any\n",
    }
    record = []
    for path, text in files.items():
        digest = base64.urlsafe_b64encode(hashlib.sha256(text.encode()).digest())
        record.append(f"{path},sha256={digest.rstrip(b'=').decode()},{len(text.encode())}\n")
    files[f"{info}/RECORD"] = "".join(record) + f"{info}/RECORD,,\n"
    with zipfile.ZipFile(f"{folder}/{name}-{version}-py3-none-any.whl", "w") as z:
        for path, text in files.items():
            z.writestr(path, text)
```

```python run file=main.py group=freeze id=freeze
import glob
import os
import subprocess
import sys
import venv

from mkwheel import make_wheel

os.mkdir("wheels")
make_wheel("wheels", "colors", "1.0", 'RED = "red"\n')
make_wheel("wheels", "greet", "2.0", 'VERSION = "2.0"\n', requires=["colors"])
venv.create("app")
pip = [sys.executable, "-m", "pip"]
subprocess.run([*pip, "--python", "app/bin/python", "install",
                "--no-index", "--find-links", "wheels", "greet"],
               check=True, capture_output=True)
site = glob.glob("app/lib/python3*/site-packages")[0]
frozen = subprocess.run([*pip, "freeze", "--path", site],
                        check=True, capture_output=True, text=True).stdout
print(frozen, end="")
with open("requirements.txt", "w", encoding="utf-8") as f:
    f.write(frozen)
```

```output for=freeze
colors==1.0
greet==2.0
```

The file `requirements.txt` here holds the output of the example [`freeze`](#freeze). The program does not copy an environment. It creates the same environment from the file in two locations, as on two machines (rule r5).

```python norun reason=fragment file=mkwheel.py group=recreate
import base64
import hashlib
import zipfile


def make_wheel(folder, name, version, code, requires=()):
    info = f"{name}-{version}.dist-info"
    meta = f"Metadata-Version: 2.1\nName: {name}\nVersion: {version}\n"
    meta += "".join(f"Requires-Dist: {r}\n" for r in requires)
    files = {
        f"{name}.py": code,
        f"{info}/METADATA": meta,
        f"{info}/WHEEL": "Wheel-Version: 1.0\nRoot-Is-Purelib: true\nTag: py3-none-any\n",
    }
    record = []
    for path, text in files.items():
        digest = base64.urlsafe_b64encode(hashlib.sha256(text.encode()).digest())
        record.append(f"{path},sha256={digest.rstrip(b'=').decode()},{len(text.encode())}\n")
    files[f"{info}/RECORD"] = "".join(record) + f"{info}/RECORD,,\n"
    with zipfile.ZipFile(f"{folder}/{name}-{version}-py3-none-any.whl", "w") as z:
        for path, text in files.items():
            z.writestr(path, text)
```

```python norun reason=fragment file=requirements.txt group=recreate
colors==1.0
greet==2.0
```

```python run file=main.py group=recreate id=recreate
import os
import subprocess
import sys
import venv

from mkwheel import make_wheel

os.mkdir("wheels")
make_wheel("wheels", "colors", "1.0", 'RED = "red"\n')
make_wheel("wheels", "greet", "2.0", 'VERSION = "2.0"\n', requires=["colors"])

targets = ["machine-a/app", "machine-b/app"]
installs = []
for env in targets:
    venv.create(env)
    installs.append(subprocess.Popen(
        [sys.executable, "-m", "pip", "--python", f"{env}/bin/python", "install",
         "--no-index", "--find-links", "wheels", "-r", "requirements.txt"],
        stdout=subprocess.DEVNULL,
    ))
assert all(p.wait() == 0 for p in installs)

code = "import colors, greet; print(greet.VERSION, colors.RED)"
for env in targets:
    out = subprocess.run([f"{env}/bin/python", "-c", code], capture_output=True, text=True)
    print(f"{env}:", out.stdout.strip())
```

```output for=recreate
machine-a/app: 2.0 red
machine-b/app: 2.0 red
```

The examples give the same output on CPython 3.12.3 with pip 24.0, and on CPython 3.13.14, 3.14.6 and 3.15.0b2 with pip 26.1.2.

## Pitfalls

### PYTHONPATH reaches into the environment

The tutorial says: "Note that the activated virtual environment does not alter the PYTHONPATH variable in any way. This may lead to unexpected results if the path includes references to code which is incompatible with the Python version the virtual environment is using."[^1] In the example, the environment imports an old copy of `greet` from a directory in `PYTHONPATH`, not the installed version.

```python norun reason=fragment file=mkwheel.py group=pythonpath
import base64
import hashlib
import zipfile


def make_wheel(folder, name, version, code, requires=()):
    info = f"{name}-{version}.dist-info"
    meta = f"Metadata-Version: 2.1\nName: {name}\nVersion: {version}\n"
    meta += "".join(f"Requires-Dist: {r}\n" for r in requires)
    files = {
        f"{name}.py": code,
        f"{info}/METADATA": meta,
        f"{info}/WHEEL": "Wheel-Version: 1.0\nRoot-Is-Purelib: true\nTag: py3-none-any\n",
    }
    record = []
    for path, text in files.items():
        digest = base64.urlsafe_b64encode(hashlib.sha256(text.encode()).digest())
        record.append(f"{path},sha256={digest.rstrip(b'=').decode()},{len(text.encode())}\n")
    files[f"{info}/RECORD"] = "".join(record) + f"{info}/RECORD,,\n"
    with zipfile.ZipFile(f"{folder}/{name}-{version}-py3-none-any.whl", "w") as z:
        for path, text in files.items():
            z.writestr(path, text)
```

```python norun reason=fragment file=legacy/greet.py group=pythonpath
VERSION = "0.9"
```

```python wrong file=main.py group=pythonpath id=pythonpath
import os
import subprocess
import sys
import venv

from mkwheel import make_wheel

os.mkdir("wheels")
make_wheel("wheels", "greet", "2.0", 'VERSION = "2.0"\n')
venv.create("app")
subprocess.run(
    [sys.executable, "-m", "pip", "--python", "app/bin/python", "install",
     "--no-index", "--find-links", "wheels", "greet"],
    check=True, capture_output=True,
)

code = "import greet; print(greet.VERSION)"
env = dict(os.environ, PYTHONPATH="legacy")
print(subprocess.run(["app/bin/python", "-c", code], env=env, capture_output=True, text=True).stdout, end="")
```

```output for=pythonpath
0.9
```

The tutorial gives the fix: "The best practice is to unset PYTHONPATH in bash or the equivalent for the shell you are using."[^1]

### An environment in version control

The venv page says that the scripts of an environment have shebang lines that "contain the absolute paths to their environment’s interpreters. Because of this, environments are inherently non-portable, in the general case."[^2] Do not commit an environment or copy it to another machine (rule r5). The packaging guide says: "You should exclude your virtual environment directory from your version control system using .gitignore or similar."[^3] Commit `requirements.txt`, and create the environment again from it.

## Comparison

**JavaScript** with npm keeps the packages of a project in the project directory. For `npm install` with no arguments in a package directory, the npm documentation says: "Install the dependencies to the local node_modules folder."[^4] A Python environment is a separate directory with its own interpreter. The venv page says: "You don’t specifically need to activate a virtual environment, as you can just specify the full path to that environment’s Python interpreter when invoking Python."[^2]

## Implementation notes

The packaging guide says of venv: "It creates a “virtual” isolated Python installation. When you switch projects, you can create a new virtual environment which is isolated from other virtual environments."[^3] The venv page explains how the interpreter knows that it runs in an environment: "When a Python interpreter is running from a virtual environment, sys.prefix and sys.exec_prefix point to the directories of the virtual environment, whereas sys.base_prefix and sys.base_exec_prefix point to those of the base Python used to create the environment."[^2] The page [[python/virtual-environments]] shows these values.

## References
