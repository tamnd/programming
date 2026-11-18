---
tag: NEW-python-build-backends-and-wheels
kind: feature
owner: python
slug: build-backends-and-wheels
name: Build backends and wheels
aliases: [build backend, build frontend, PEP 517, build_wheel, build_sdist, build_editable, backend-path, in-tree backend, build isolation, wheel, .whl, sdist, source distribution, editable install, pip install -e, RECORD file, compatibility tags, py3-none-any]
area: languages/python/modules-and-packages
level: advanced
summary: >-
  A frontend such as pip calls the PEP 517 hooks of the build backend named in pyproject.toml. The hooks write wheels, which are ZIP files with tags and hashes, and source distributions.
grammar: null
spec: {standard: null, url: "https://peps.python.org/pep-0517/", locator: "Build backend interface; Build environment; In-tree build backends"}
see_also: [python/pyproject-toml, python/managing-packages-with-pip, python/virtual-environments]
sources:
- url: https://peps.python.org/pep-0517/
  title: "PEP 517: A build-system independent format for source trees"
  locator: Terminology and goals; Source trees; Build backend interface; Build environment; In-tree build backends
  accessed: 2026-11-18
- url: https://packaging.python.org/en/latest/specifications/binary-distribution-format/
  title: Binary distribution format, Python Packaging User Guide
  locator: Installing a wheel; File name convention; The .dist-info directory
  accessed: 2026-11-18
- url: https://packaging.python.org/en/latest/specifications/platform-compatibility-tags/
  title: Platform compatibility tags, Python Packaging User Guide
  locator: Overview; Use
  accessed: 2026-11-18
- url: https://packaging.python.org/en/latest/specifications/source-distribution-format/
  title: Source distribution format, Python Packaging User Guide
  locator: Source trees; Source distribution file name; Source distribution file format
  accessed: 2026-11-18
- url: https://peps.python.org/pep-0660/
  title: "PEP 660: Editable installs for pyproject.toml based builds (wheel based)"
  locator: Abstract; Motivation; build_editable; What to put in the wheel
  accessed: 2026-11-18
- url: https://pip.pypa.io/en/stable/cli/pip_install/
  title: pip install, pip documentation
  locator: Options
  accessed: 2026-11-18
- url: https://go.dev/ref/mod
  title: Go Modules Reference
  locator: Module zip files
  accessed: 2026-11-18
---

## Synopsis

```text
# pyproject.toml
[build-system]
requires = ["hatchling >= 1.26"]        # installed into the build environment
build-backend = "hatchling.build"       # module or module:object
backend-path = ["."]                    # only for a backend in the source tree

# the hooks of the backend object
build_wheel(wheel_directory, config_settings=None, metadata_directory=None) -> "name-1.0-py3-none-any.whl"
build_sdist(sdist_directory, config_settings=None) -> "name-1.0.tar.gz"
build_editable(wheel_directory, config_settings=None, metadata_directory=None) -> "name-1.0-py3-none-any.whl"

# frontends
python -m pip wheel ./project           # build a wheel
python -m pip install ./project         # build a wheel and install it
python -m pip install -e ./project      # editable install
```

## Semantics

**r1.** PEP 517 separates two tools. "A build frontend is a tool that users might run that takes arbitrary source trees or source distributions and builds wheels from them. The actual building is done by each source tree’s build backend. In a command like pip wheel some-directory/, pip is acting as a build frontend."[^1] Example: [`install`](#install).

**r2.** "build-backend is a string naming a Python object that will be used to perform the build (see below for details). This is formatted following the same module:object syntax as a setuptools entry point."[^1] "The build backend object is expected to have attributes which provide some or all of the following hooks."[^1] "All hooks are run with working directory set to the root of the source tree"[^1]. Example: [`hooks`](#hooks).

**r3.** Two hooks are mandatory. `build_wheel` "Must build a .whl file, and place it in the specified wheel_directory. It must return the basename (not the full path) of the .whl file it creates, as a unicode string."[^1] `build_sdist` "Must build a .tar.gz source distribution and place it in the specified sdist_directory."[^1] Other hooks are optional. For `get_requires_for_build_wheel`: "If not defined, the default implementation is equivalent to return []."[^1] Example: [`hooks`](#hooks).

**r4.** "A build frontend SHOULD, by default, create an isolated environment for each build, containing only the standard library and any explicitly requested build-dependencies."[^1] The `requires` key of `[build-system]` lists those dependencies. With `requires = []`, the build environment has only the standard library. Example: [`install`](#install).

**r5.** A project can keep its backend in the source tree. The `backend-path` key "contains a list of directories, which the frontend will add to the start of sys.path when loading the backend, and running the backend hooks."[^1] Without it, the frontend does not look in the source tree: "When importing the module path, we do not look in the directory containing the source tree, unless that would be on sys.path anyway (e.g. because it is specified in PYTHONPATH)."[^1] Examples: [`install`](#install), [`no-backend-path`](#no-backend-path).

**r6.** "A wheel is a ZIP-format archive with a specially formatted file name and the .whl extension."[^2] "The wheel filename is {distribution}-{version}(-{build tag})?-{python tag}-{abi tag}-{platform tag}.whl."[^2] For the tags: "For example, the tag py27-none-any indicates compatibility with Python 2.7 (any Python 2.7 implementation) with no abi requirement, on any platform."[^3] Example: [`hooks`](#hooks).

**r7.** The `.dist-info` directory of a wheel holds the metadata. "RECORD is a list of (almost) all the files in the wheel and their secure hashes. Unlike PEP 376, every file except RECORD, which cannot contain a hash of itself, must include its hash. The hash algorithm must be sha256 or better; specifically, md5 and sha1 are not permitted."[^2] To install a pure wheel, the installer does this: "If Root-Is-Purelib == ‘true’, unpack archive into purelib (site-packages)."[^2] Example: [`hooks`](#hooks).

**r8.** "A .tar.gz source distribution (sdist) contains a single top-level directory called {name}-{version} (e.g. foo-1.0), containing the source files of the package."[^4] "This directory must also contain a pyproject.toml in the format defined in pyproject.toml specification, and a PKG-INFO file containing metadata in the format described in the Core metadata specifications specification."[^4] Example: [`hooks`](#hooks).

**r9.** PEP 660 adds editable installs: "This document describes a PEP 517 style method for the installation of packages in editable mode."[^5] The hook `build_editable` also "Must build a .whl file, and place it in the specified wheel_directory."[^5] One technique is a path file: "Build backends may choose to place a .pth file at the root of the .whl file, containing the root directory of the source tree."[^5] The pip option `-e` will "Install a project in editable mode (i.e. setuptools “develop mode”) from a local project path or a VCS url."[^6] Example: [`install`](#install).

## Examples

The project `proj` has its own small backend, `backend.py`. The backend uses only the standard library, so `requires` is empty and no network is needed (rules r4 and r5). It reads `[project]` with `tomllib` and writes a wheel, an editable wheel and an sdist.

```python norun reason=fragment file=proj/pyproject.toml group=hooks
[build-system]
requires = []
build-backend = "backend"
backend-path = ["."]

[project]
name = "greet"
version = "1.0"
```

```python norun reason=fragment file=proj/greet.py group=hooks
def hello():
    return "hello 1"
```

```python norun reason=fragment file=proj/backend.py group=hooks
import base64
import hashlib
import io
import os
import tarfile
import tomllib
import zipfile

with open("pyproject.toml", "rb") as f:
    PROJECT = tomllib.load(f)["project"]
NAME, VERSION = PROJECT["name"], PROJECT["version"]
INFO = f"{NAME}-{VERSION}.dist-info"
METADATA = f"Metadata-Version: 2.2\nName: {NAME}\nVersion: {VERSION}\n"
WHEEL = "Wheel-Version: 1.0\nGenerator: backend\nRoot-Is-Purelib: true\nTag: py3-none-any\n"


def _write_wheel(directory, files):
    files = {**files, f"{INFO}/METADATA": METADATA, f"{INFO}/WHEEL": WHEEL}
    record = ""
    for path, text in files.items():
        data = text.encode()
        digest = base64.urlsafe_b64encode(hashlib.sha256(data).digest()).rstrip(b"=")
        record += f"{path},sha256={digest.decode()},{len(data)}\n"
    files[f"{INFO}/RECORD"] = record + f"{INFO}/RECORD,,\n"
    name = f"{NAME}-{VERSION}-py3-none-any.whl"
    with zipfile.ZipFile(os.path.join(directory, name), "w") as z:
        for path, text in files.items():
            z.writestr(path, text)
    return name


def build_wheel(wheel_directory, config_settings=None, metadata_directory=None):
    with open(f"{NAME}.py", encoding="utf-8") as f:
        return _write_wheel(wheel_directory, {f"{NAME}.py": f.read()})


def build_editable(wheel_directory, config_settings=None, metadata_directory=None):
    # A .pth file with the source directory (PEP 660 names this technique).
    return _write_wheel(wheel_directory, {f"_{NAME}_editable.pth": os.getcwd() + "\n"})


def build_sdist(sdist_directory, config_settings=None):
    name = f"{NAME}-{VERSION}.tar.gz"
    with tarfile.open(os.path.join(sdist_directory, name), "w:gz") as tar:
        for path in ["pyproject.toml", "backend.py", f"{NAME}.py"]:
            tar.add(path, f"{NAME}-{VERSION}/{path}")
        info = tarfile.TarInfo(f"{NAME}-{VERSION}/PKG-INFO")
        info.size = len(METADATA.encode())
        tar.addfile(info, io.BytesIO(METADATA.encode()))
    return name
```

A frontend imports the backend with `backend-path` at the start of `sys.path` and calls the hooks in the source tree (rules r2 and r5). This program does the same steps without pip. The wheel name has the tags `py3-none-any` (rule r6). RECORD has a sha256 hash for each file except itself (rule r7). The sdist has one top-level directory with `pyproject.toml` and `PKG-INFO` (rule r8).

```python run file=main.py group=hooks id=hooks
import os
import sys
import tarfile
import zipfile

os.mkdir("dist")
os.chdir("proj")
sys.path.insert(0, os.getcwd())
import backend

wheel = backend.build_wheel("../dist")
sdist = backend.build_sdist("../dist")
with zipfile.ZipFile(f"../dist/{wheel}") as z:
    print(wheel, z.namelist())
    for line in z.read("greet-1.0.dist-info/RECORD").decode().splitlines():
        print("  ", line[:40] + ("..." if len(line) > 40 else ""))
with tarfile.open(f"../dist/{sdist}") as tar:
    print(sdist, tar.getnames())
```

```output for=hooks
greet-1.0-py3-none-any.whl ['greet.py', 'greet-1.0.dist-info/METADATA', 'greet-1.0.dist-info/WHEEL', 'greet-1.0.dist-info/RECORD']
   greet.py,sha256=oDQQqozyuRhcuaYZiOF84LJD...
   greet-1.0.dist-info/METADATA,sha256=2HFG...
   greet-1.0.dist-info/WHEEL,sha256=o_Wk-Ao...
   greet-1.0.dist-info/RECORD,,
greet-1.0.tar.gz ['greet-1.0/pyproject.toml', 'greet-1.0/backend.py', 'greet-1.0/greet.py', 'greet-1.0/PKG-INFO']
```

pip is the frontend here (rule r1). It installs the project into `env` with `build_wheel` and into `dev` with `build_editable` (rule r9). Then the program changes `greet.py` in the source tree. Only the editable install sees the change, because its `.pth` file puts `proj` on `sys.path`.

```python norun reason=fragment file=proj/pyproject.toml group=install
[build-system]
requires = []
build-backend = "backend"
backend-path = ["."]

[project]
name = "greet"
version = "1.0"
```

```python norun reason=fragment file=proj/greet.py group=install
def hello():
    return "hello 1"
```

```python norun reason=fragment file=proj/backend.py group=install
import base64
import hashlib
import io
import os
import tarfile
import tomllib
import zipfile

with open("pyproject.toml", "rb") as f:
    PROJECT = tomllib.load(f)["project"]
NAME, VERSION = PROJECT["name"], PROJECT["version"]
INFO = f"{NAME}-{VERSION}.dist-info"
METADATA = f"Metadata-Version: 2.2\nName: {NAME}\nVersion: {VERSION}\n"
WHEEL = "Wheel-Version: 1.0\nGenerator: backend\nRoot-Is-Purelib: true\nTag: py3-none-any\n"


def _write_wheel(directory, files):
    files = {**files, f"{INFO}/METADATA": METADATA, f"{INFO}/WHEEL": WHEEL}
    record = ""
    for path, text in files.items():
        data = text.encode()
        digest = base64.urlsafe_b64encode(hashlib.sha256(data).digest()).rstrip(b"=")
        record += f"{path},sha256={digest.decode()},{len(data)}\n"
    files[f"{INFO}/RECORD"] = record + f"{INFO}/RECORD,,\n"
    name = f"{NAME}-{VERSION}-py3-none-any.whl"
    with zipfile.ZipFile(os.path.join(directory, name), "w") as z:
        for path, text in files.items():
            z.writestr(path, text)
    return name


def build_wheel(wheel_directory, config_settings=None, metadata_directory=None):
    with open(f"{NAME}.py", encoding="utf-8") as f:
        return _write_wheel(wheel_directory, {f"{NAME}.py": f.read()})


def build_editable(wheel_directory, config_settings=None, metadata_directory=None):
    # A .pth file with the source directory (PEP 660 names this technique).
    return _write_wheel(wheel_directory, {f"_{NAME}_editable.pth": os.getcwd() + "\n"})


def build_sdist(sdist_directory, config_settings=None):
    name = f"{NAME}-{VERSION}.tar.gz"
    with tarfile.open(os.path.join(sdist_directory, name), "w:gz") as tar:
        for path in ["pyproject.toml", "backend.py", f"{NAME}.py"]:
            tar.add(path, f"{NAME}-{VERSION}/{path}")
        info = tarfile.TarInfo(f"{NAME}-{VERSION}/PKG-INFO")
        info.size = len(METADATA.encode())
        tar.addfile(info, io.BytesIO(METADATA.encode()))
    return name
```

```python run file=main.py group=install id=install
import subprocess
import sys
import venv

pip = [sys.executable, "-m", "pip", "--python"]
venv.create("env")
venv.create("dev")
subprocess.run([*pip, "env/bin/python", "install", "--no-index", "./proj"],
               check=True, capture_output=True)
subprocess.run([*pip, "dev/bin/python", "install", "--no-index", "-e", "./proj"],
               check=True, capture_output=True)

with open("proj/greet.py", "w", encoding="utf-8") as f:
    f.write('def hello():\n    return "hello 2"\n')

code = "import greet, importlib.util; print(greet.hello(), importlib.util.find_spec('backend') is not None)"
for env in ["env", "dev"]:
    out = subprocess.run([f"{env}/bin/python", "-c", code], capture_output=True, text=True)
    print(env, out.stdout.strip())
```

```output for=install
env hello 1 False
dev hello 2 True
```

The last value tells if the module `backend` can be imported. See the pitfall [Editable installs show extra modules](#editable-installs-show-extra-modules).

## Pitfalls

### An in-tree backend without backend-path

The frontend does not add the source tree to `sys.path` (rule r5). Without `backend-path`, pip cannot import `backend`, and the install fails. With `check=True`, `subprocess.run` raises `CalledProcessError`.

```python norun reason=fragment file=proj/pyproject.toml group=no-backend-path
[build-system]
requires = []
build-backend = "backend"

[project]
name = "greet"
version = "1.0"
```

```python norun reason=fragment file=proj/backend.py group=no-backend-path
def build_wheel(wheel_directory, config_settings=None, metadata_directory=None):
    raise NotImplementedError


def build_sdist(sdist_directory, config_settings=None):
    raise NotImplementedError
```

```python fails file=main.py group=no-backend-path id=no-backend-path expect=exception:CalledProcessError
import subprocess
import sys
import venv

venv.create("env")
subprocess.run([sys.executable, "-m", "pip", "--python", "env/bin/python",
                "install", "--no-index", "./proj"], check=True, capture_output=True)
```

### Editable installs show extra modules

A `.pth` file puts the whole source directory on `sys.path`. In the example [`install`](#install), `backend` can be imported in `dev` but not in `env`. PEP 660 says: "Path-based methods make all scripts under a path importable, often including the project’s own setup.py and other scripts that would not be part of a normal installation."[^5] Keep the package in its own directory, such as `src`, so that the path holds only the package.

### Metadata from two hooks

The metadata of the wheel must stay the same. If the frontend gave `metadata_directory`, then "build_wheel MUST produce a wheel with identical metadata."[^1] A backend that computes metadata, for example from the time of the build, must give the same result each time.

## Comparison

**Go** has no build backend. "Module versions are distributed as .zip files."[^7] The go command can "clone source control repositories and create zip files from them."[^7] A Python project can ship built wheels, and the backend named in `pyproject.toml` builds them (rules r1 and r6).

## Implementation notes

The example backend is a sketch. It does not handle `config_settings`, data files or scripts, and it writes core metadata with only three fields. A wheel can also be installed without an installer: "a wheel file may be installed by simply unpacking into site-packages with the standard ‘unzip’ tool while preserving enough information to spread its contents out onto their final paths at any later time."[^2] The file name of an sdist is fixed: "The file name must be in the form {name}-{version}.tar.gz"[^4]. Isolation lets one install build packages with different needs: "It allows for a single installation run to build multiple packages that have contradictory build-requirements."[^1]

## References
