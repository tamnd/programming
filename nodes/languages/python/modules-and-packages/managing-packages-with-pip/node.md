---
tag: NEW-python-managing-packages-with-pip
kind: feature
owner: python
slug: managing-packages-with-pip
name: Managing Packages with pip
aliases: [pip, python -m pip, pip install, pip uninstall, pip show, pip list, pip freeze, requirements file, requirements.txt, pip install -r, pip install --upgrade, --find-links, externally managed environment]
area: languages/python/modules-and-packages
level: intro
summary: >-
  python -m pip installs, upgrades and removes packages in the environment of that Python. pip show and pip list describe them, and pip freeze writes pinned requirements for pip install -r.
grammar: null
spec: {standard: null, url: "https://pip.pypa.io/en/stable/user_guide/", locator: "Running pip; Installing Packages; Requirements Files; Uninstalling Packages; Listing Packages"}
see_also: [python/virtual-environments-and-packages, python/virtual-environments, python/pyproject-toml, python/build-backends-and-wheels]
sources:
- url: https://docs.python.org/3.15/tutorial/venv.html#managing-packages-with-pip
  title: The Python Tutorial, 12. Virtual Environments and Packages
  locator: 12.3. Managing Packages with pip
  accessed: 2026-11-18
- url: https://pip.pypa.io/en/stable/user_guide/
  title: User Guide, pip documentation
  locator: Running pip; Requirements Files; Uninstalling Packages
  accessed: 2026-11-18
- url: https://pip.pypa.io/en/stable/cli/pip_install/
  title: pip install, pip documentation
  locator: Description; Overview; Options
  accessed: 2026-11-18
- url: https://pip.pypa.io/en/stable/cli/pip_freeze/
  title: pip freeze, pip documentation
  locator: Description
  accessed: 2026-11-18
- url: https://pip.pypa.io/en/stable/cli/pip_uninstall/
  title: pip uninstall, pip documentation
  locator: Description; Options
  accessed: 2026-11-18
- url: https://pip.pypa.io/en/stable/cli/pip_show/
  title: pip show, pip documentation
  locator: Description
  accessed: 2026-11-18
- url: https://pip.pypa.io/en/stable/cli/pip_list/
  title: pip list, pip documentation
  locator: Description
  accessed: 2026-11-18
- url: https://pip.pypa.io/en/stable/reference/requirements-file-format/
  title: Requirements File Format, pip documentation
  locator: Requirements File Format; Example
  accessed: 2026-11-18
- url: https://pip.pypa.io/en/stable/cli/pip/
  title: pip, pip documentation
  locator: General Options
  accessed: 2026-11-18
- url: https://packaging.python.org/en/latest/specifications/externally-managed-environments/
  title: Externally Managed Environments, Python Packaging User Guide
  locator: Externally Managed Environments
  accessed: 2026-11-18
- url: https://docs.npmjs.com/cli/v11/commands/npm-install
  title: npm-install, npm Docs
  locator: Description; Configuration
  accessed: 2026-11-18
---

## Synopsis

```text
python -m pip install requests             # the latest version that fits
python -m pip install requests==2.6.0      # a version specifier
python -m pip install --upgrade requests
python -m pip install -r requirements.txt
python -m pip install --no-index --find-links=wheels/ requests   # no index, a local directory

python -m pip uninstall -y requests
python -m pip show requests                # metadata of one package
python -m pip list                         # all packages
python -m pip freeze > requirements.txt    # pinned name==version lines
python -m pip --python .venv/bin/python list   # act on the environment of another Python
```

## Semantics

**r1.** pip acts on one Python. "python -m pip executes pip using the Python interpreter you specified as python. So /usr/bin/python3.7 -m pip means you are executing pip for your interpreter located at /usr/bin/python3.7."[^2] The general option `--python <python>` tells pip to "Run pip with the specified Python interpreter."[^9] Examples: [`install-upgrade`](#install-upgrade), [`show-list`](#show-list).

**r2.** `pip install` takes names with version specifiers, local files and directories, and requirements files.[^3] The tutorial says: "You can install the latest version of a package by specifying a package’s name" and "You can also install a specific version of a package by giving the package name followed by == and the version number".[^1] pip also installs the dependencies of each package. Its stages are: "Identify the base requirements. The user supplied arguments are processed here. Resolve dependencies. What will be installed is determined here. Build wheels. All the dependencies that can be are built into wheels. Install the packages (and uninstall anything being upgraded/replaced)."[^3] Examples: [`install-upgrade`](#install-upgrade), [`show-list`](#show-list).

**r3.** "Note that pip install prefers to leave the installed version as-is unless --upgrade is specified."[^3] The tutorial says that if you run the install again, "pip will notice that the requested version is already installed and do nothing."[^1] Examples: [`install-upgrade`](#install-upgrade), [`upgrade`](#upgrade).

**r4.** pip looks for packages in an index, and by default that is the Python Package Index.[^1] `--no-index` tells pip to "Ignore package index (only looking at --find-links URLs instead)."[^3] With `--find-links` and "a local path or file:// URL that’s a directory, then look for archives in the directory listing."[^3] Every example uses these two options.

**r5.** Three commands describe the environment. `pip show` will "Show information about one or more installed packages. The output is in RFC-compliant mail header format."[^6] `pip list` will "List installed packages, including editables. Packages are listed in a case-insensitive sorted order."[^7] `pip freeze` will "Output installed packages in requirements format. packages are listed in a case-insensitive sorted order."[^4] Example: [`show-list`](#show-list).

**r6.** "Logically, a Requirements file is just a list of pip install arguments placed in a file. Note that you should not rely on the items in the file being installed by pip in any particular order."[^2] A line can be a requirement or an option, and "#-prefixed lines are ignored".[^8] The user guide says that requirements files "are used to hold the result from pip freeze for the purpose of achieving Repeatable Installs."[^2] Example: [`requirements`](#requirements).

**r7.** `pip uninstall` removes the named packages: "Uninstall packages. pip is able to uninstall most installed packages."[^5] The option `-y` means "Don’t ask for confirmation of uninstall deletions."[^5] Example: [`uninstall`](#uninstall).

## Examples

The examples do not use the network. The helper module `mkwheel.py` writes wheel files into the directory `wheels`, and pip installs from there (rule r4). The page [[python/build-backends-and-wheels]] explains the wheel format. Each example makes an environment with `venv.create("env")` and runs the pip of the outer Python with `--python env/bin/python` (rule r1).

`pip install greet==1.5` installs that version. A second `pip install greet` changes nothing (rules r2 and r3).

```python norun reason=fragment file=mkwheel.py group=install-upgrade
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

```python run file=main.py group=install-upgrade id=install-upgrade
import glob
import os
import subprocess
import sys
import venv
from importlib.metadata import distributions

from mkwheel import make_wheel

os.mkdir("wheels")
for version in ["1.0", "1.5", "2.0"]:
    make_wheel("wheels", "greet", version, f'VERSION = "{version}"\n')
venv.create("env")
site = glob.glob("env/lib/python3*/site-packages")


def pip(*args):
    command = [sys.executable, "-m", "pip", "--python", "env/bin/python", *args]
    subprocess.run(command, check=True, capture_output=True)
    versions = [d.version for d in distributions(path=site) if d.name == "greet"]
    print(" ".join(args[:2]), "->", *versions)


pip("install", "greet==1.5", "--no-index", "--find-links", "wheels")
pip("install", "greet", "--no-index", "--find-links", "wheels")
```

```output for=install-upgrade
install greet==1.5 -> 1.5
install greet -> 1.5
```

With `--upgrade`, the same install gets the latest version (rule r3).

```python norun reason=fragment file=mkwheel.py group=upgrade
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

```python run file=main.py group=upgrade id=upgrade
import glob
import os
import subprocess
import sys
import venv
from importlib.metadata import distributions

from mkwheel import make_wheel

os.mkdir("wheels")
for version in ["1.0", "1.5", "2.0"]:
    make_wheel("wheels", "greet", version, f'VERSION = "{version}"\n')
venv.create("env")
site = glob.glob("env/lib/python3*/site-packages")


def pip(*args):
    command = [sys.executable, "-m", "pip", "--python", "env/bin/python", *args]
    subprocess.run(command, check=True, capture_output=True)
    versions = [d.version for d in distributions(path=site) if d.name == "greet"]
    print(" ".join(args[:2]), "->", *versions)


pip("install", "greet==1.5", "--no-index", "--find-links", "wheels")
pip("install", "--upgrade", "greet", "--no-index", "--find-links", "wheels")
```

```output for=upgrade
install greet==1.5 -> 1.5
install --upgrade -> 2.0
```

`greet` 2.0 depends on `colors`, so pip installs both (rule r2). `pip show` gives the metadata as header lines, and `pip list` gives every package (rule r5).

```python norun reason=fragment file=mkwheel.py group=show-list
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

```python run file=main.py group=show-list id=show-list
import json
import os
import subprocess
import sys
import venv

from mkwheel import make_wheel

os.mkdir("wheels")
make_wheel("wheels", "colors", "1.0", 'RED = "red"\n')
make_wheel("wheels", "greet", "2.0", 'VERSION = "2.0"\n', requires=["colors>=1.0"])
venv.create("env")


def pip(*args):
    command = [sys.executable, "-m", "pip", "--python", "env/bin/python", *args]
    return subprocess.run(command, check=True, capture_output=True, text=True).stdout


pip("install", "greet", "--no-index", "--find-links", "wheels")
for line in pip("show", "greet", "colors").splitlines():
    if line.startswith(("Name:", "Version:", "Requires:", "Required-by:", "---")):
        print(line.rstrip())
print(json.loads(pip("list", "--format=json")))
```

```output for=show-list
Name: greet
Version: 2.0
Requires: colors
Required-by:
---
Name: colors
Version: 1.0
Requires:
Required-by: greet
[{'name': 'colors', 'version': '1.0'}, {'name': 'greet', 'version': '2.0'}]
```

A requirements file holds requirements, comments and options (rule r6). Here the file itself has the options `--no-index` and `--find-links`. `pip freeze` then gives the pinned versions.

```python norun reason=fragment file=mkwheel.py group=requirements
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

```python norun reason=fragment file=requirements.txt group=requirements
# Options can be in the file too.
--no-index
--find-links wheels

greet>=1.0,<2    # the latest 1.x
colors==1.0
```

```python run file=main.py group=requirements id=requirements
import os
import subprocess
import sys
import venv

from mkwheel import make_wheel

os.mkdir("wheels")
make_wheel("wheels", "colors", "1.0", 'RED = "red"\n')
for version in ["1.0", "1.5", "2.0"]:
    make_wheel("wheels", "greet", version, f'VERSION = "{version}"\n')
venv.create("env")


def pip(*args):
    command = [sys.executable, "-m", "pip", "--python", "env/bin/python", *args]
    return subprocess.run(command, check=True, capture_output=True, text=True).stdout


pip("install", "-r", "requirements.txt")
print(pip("freeze"), end="")
```

```output for=requirements
colors==1.0
greet==1.5
```

The examples give the same output on CPython 3.12.3 with pip 24.0, and on CPython 3.13.14, 3.14.6 and 3.15.0b2 with pip 26.1.2.

## Pitfalls

### pip of another Python

`pip` on the command line can belong to a different Python than `python` (rule r1). Then the package goes into another environment. Run `python -m pip`, or activate the environment first, so that pip and the program use the same Python.

### Uninstall leaves the dependencies

`pip uninstall` removes only the packages that you name (rule r7). Here `colors` stays after `greet` is removed.

```python norun reason=fragment file=mkwheel.py group=uninstall
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

```python run file=main.py group=uninstall id=uninstall
import os
import subprocess
import sys
import venv

from mkwheel import make_wheel

os.mkdir("wheels")
make_wheel("wheels", "colors", "1.0", 'RED = "red"\n')
make_wheel("wheels", "greet", "2.0", 'VERSION = "2.0"\n', requires=["colors"])
venv.create("env")


def pip(*args):
    command = [sys.executable, "-m", "pip", "--python", "env/bin/python", *args]
    return subprocess.run(command, check=True, capture_output=True, text=True).stdout


pip("install", "greet", "--no-index", "--find-links", "wheels")
pip("uninstall", "-y", "greet")
print(pip("freeze"), end="")
```

```output for=uninstall
colors==1.0
```

To remove a dependency too, name it, or create the environment again from a requirements file.

### No version fits

pip stops with an error when no file fits the requirement (rule r2). With `check=True`, `subprocess.run` raises `CalledProcessError`.

```python norun reason=fragment file=mkwheel.py group=no-match
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

```python fails file=main.py group=no-match id=no-match expect=exception:CalledProcessError
import os
import subprocess
import sys
import venv

from mkwheel import make_wheel

os.mkdir("wheels")
make_wheel("wheels", "greet", "2.0", 'VERSION = "2.0"\n')
venv.create("env")
command = [sys.executable, "-m", "pip", "--python", "env/bin/python",
           "install", "greet==3.0", "--no-index", "--find-links", "wheels"]
result = subprocess.run(command, capture_output=True, text=True)
print(result.stderr.strip().splitlines()[-1])
result.check_returncode()
```

### An externally managed Python

A Python of the operating system can refuse pip. The specification says that a marker file lets "a Python installation to indicate to Python-specific tools such as pip that they neither install nor remove packages into the interpreter’s default installation environment, and should instead guide the end user towards using Python Virtual Environments."[^10] Install into an environment (see [[python/virtual-environments]]).

## Comparison

**JavaScript** with npm records each installed package in the project file: "npm install saves any specified packages into dependencies by default."[^11] `pip install` changes only the environment. The project must write its requirements itself, in a requirements file or in `pyproject.toml` (see [[python/pyproject-toml]]).

## Implementation notes

`pip freeze` lists what is installed, not a resolved plan: "pip freeze reports what is installed; it does not compute a lockfile or a solver result."[^4] It leaves out pip itself: "By default, pip freeze omits bootstrap packaging tools so the output focuses on your project’s dependencies."[^4] The requirements format belongs to pip: "the full syntax, as described here, is only intended for consumption by pip, and other tools should take that into account before using it for their own purposes."[^8] Before an upgrade, pip removes the old version: "Pip also performs an automatic uninstall of an old version of a package before upgrading to a newer version."[^2]

## References
