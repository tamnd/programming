---
tag: 70HN3M
kind: tool
slug: pip-tool
name: pip
aliases: [pip, pip install, pip3, python -m pip, requirements.txt, constraints file, pip freeze, hash-checking mode]
area: tools-and-workflow/package-managers
level: core
summary: >-
  pip is the package installer for Python. This page shows virtual environments, requirements files, constraints, hash-checking mode, wheels, the configuration and the history of the tool.
role: [package-manager]
licence: mit
repo: https://github.com/pypa/pip
homepage: https://pip.pypa.io/
see_also: [package-manager-concept, python, version-constraints, pin-dependency-versions, lock-files, dependency-resolution, package-registries, add-a-dependency]
sources:
  - url: https://pip.pypa.io/en/stable/
    title: pip documentation, pip
    locator: the introduction
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/user_guide/
    title: pip documentation, User Guide
    locator: Requirements Files; Constraints Files
    accessed: 2026-11-16
  - url: https://github.com/pypa/pip/blob/main/LICENSE.txt
    title: pip, LICENSE.txt
    locator: the whole file
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/news/
    title: pip documentation, Changelog
    locator: 25.1 (2025-04-26); 26.1 (2026-04-26); 26.3 (2026-11-02)
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/installation/
    title: pip documentation, Installation
    locator: the introduction; ensurepip
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/venv.html
    title: Python documentation, venv
    locator: the introduction
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/specifications/externally-managed-environments/
    title: Python Packaging User Guide, Externally Managed Environments
    locator: the abstract
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/specifications/binary-distribution-format/
    title: Python Packaging User Guide, Binary distribution format
    locator: the introduction
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/topics/secure-installs/
    title: pip documentation, Secure installs
    locator: the introduction; Hash-checking Mode; Forcing Hash-checking mode
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/reference/requirements-file-format/
    title: pip documentation, Requirements File Format
    locator: the introduction; Example
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/cli/pip_install/
    title: pip documentation, pip install
    locator: Options (-r, --requirement)
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/cli/pip_freeze/
    title: pip documentation, pip freeze
    locator: Description
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/topics/repeatable-installs/
    title: pip documentation, Repeatable Installs
    locator: Pinning the package versions
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/topics/dependency-resolution/
    title: pip documentation, Dependency Resolution
    locator: Backtracking
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/topics/configuration/
    title: pip documentation, Configuration
    locator: the introduction; Configuration Files; Environment Variables
    accessed: 2026-11-16
  - url: https://pypi.org/pypi/pip/json
    title: PyPI, pip (JSON API)
    locator: releases (upload_time of 0.2 and 1.0)
    accessed: 2026-11-16
---

## What it is

"Pip is the package installer for Python. You can use it to install packages from the Python Package Index and other indexes."[^1] The Python Package Index, PyPI, is the public registry of Python; see [[package-registries]]. pip installs the packages into one Python environment, and it reads the dependencies of each package from its metadata.[^2] pip is not a project manager: a page of its documentation has the title "Pip is not a workflow management tool".[^1] See [[python]].

pip has the MIT licence.[^3] The newest release on 2026-11-12 was pip 26.3, of 2026-11-02.[^4] pip 26.1 dropped "support for Python 3.9".[^4] The outputs on this page come from pip 24.0 on Python 3.12.

## Install and first use

pip is "automatically installed" in a virtual environment and in a Python from python.org.[^5] If it is not there, `python -m ensurepip --upgrade` installs it.[^5] Run pip as `python3 -m pip`, so that it works with the same Python as the program.

The script makes a small wheel by hand, so that it needs no network. Then it makes a virtual environment, pins the package with its hash in `requirements.txt`, installs it from a local directory, and shows what pip knows about it. Then it tries a file with a wrong hash, and pip refuses it.

```bash run timeout=30 id=pip-first
d=$(mktemp -d)
cd "$d"
export PIP_DISABLE_PIP_VERSION_CHECK=1 PIP_NO_CACHE_DIR=1
# Make a small wheel by hand: a zip file with the code and a .dist-info directory.
mkdir wheels
python3 - <<'END'
import base64, hashlib, zipfile
files = {
    "greet/__init__.py": 'def hello(name):\n    return f"Hello, {name}!"\n',
    "greet-1.0.0.dist-info/METADATA": "Metadata-Version: 2.1\nName: greet\nVersion: 1.0.0\n",
    "greet-1.0.0.dist-info/WHEEL": "Wheel-Version: 1.0\nGenerator: by-hand\nRoot-Is-Purelib: true\nTag: py3-none-any\n",
}
record = []
with zipfile.ZipFile("wheels/greet-1.0.0-py3-none-any.whl", "w") as z:
    for name, text in files.items():
        data = text.encode()
        z.writestr(name, data)
        digest = base64.urlsafe_b64encode(hashlib.sha256(data).digest()).rstrip(b"=").decode()
        record.append(f"{name},sha256={digest},{len(data)}")
    record.append("greet-1.0.0.dist-info/RECORD,,")
    z.writestr("greet-1.0.0.dist-info/RECORD", "\n".join(record) + "\n")
END
# A virtual environment keeps the packages of this project apart.
python3 -m venv .venv
. .venv/bin/activate
# Pin the version and the hash in a requirements file.
h=$(python3 -m pip hash wheels/greet-1.0.0-py3-none-any.whl | tail -1)
printf 'greet==1.0.0 %s\n' "$h" >requirements.txt
sed 's/sha256:.*/sha256:.../' requirements.txt
python3 -m pip install --quiet --no-index --find-links wheels --require-hashes -r requirements.txt
python3 -c 'import greet; print(greet.hello("pip"))'
python3 -m pip freeze
python3 -m pip show greet | grep -E '^(Name|Version|Location)' | sed "s|$d|DIR|"
# A wrong hash stops the install.
printf 'greet==1.0.0 --hash=sha256:%064d\n' 0 >bad.txt
python3 -m pip install --quiet --force-reinstall --no-index --find-links wheels -r bad.txt 2>&1 | sed "s|$d|DIR|" | grep -v "^ *Got" | sed "/^$/d"
python3 -m pip uninstall --yes --quiet greet
python3 -c 'import importlib.util; print(importlib.util.find_spec("greet"))'
```

```output for=pip-first
greet==1.0.0 --hash=sha256:...
Hello, pip!
greet==1.0.0
Name: greet
Version: 1.0.0
Location: DIR/.venv/lib/python3.12/site-packages
ERROR: THESE PACKAGES DO NOT MATCH THE HASHES FROM THE REQUIREMENTS FILE. If you have updated the package versions, please update the hashes. Otherwise, examine the package contents carefully; someone may have tampered with them.
    greet==1.0.0 from file://DIR/wheels/greet-1.0.0-py3-none-any.whl (from -r bad.txt (line 1)):
        Expected sha256 0000000000000000000000000000000000000000000000000000000000000000
None
```

## Concepts of the tool

**Virtual environments.** The module `venv` creates "lightweight “virtual environments”, each with their own independent set of Python packages installed in their site directories".[^6] Use one environment for each project. A Python installation, such as the system Python of a Linux distribution, can have the marker file `EXTERNALLY-MANAGED`. It tells tools such as pip "that they neither install nor remove packages into the interpreter’s default installation environment", and that they "should instead guide the end user towards using Python Virtual Environments".[^7]

**Wheels and source distributions.** "A wheel is a ZIP-format archive with a specially formatted file name and the .whl extension."[^8] A wheel "contains a single distribution nearly as it would be installed".[^8] A source distribution must be built first, and by default an install "involves running arbitrary code from distributions".[^9] The script above makes a wheel with the three files of the `.dist-info` directory: `METADATA`, `WHEEL` and `RECORD`.

**Requirements files.** "Requirements files serve as a list of items to be installed by pip, when using pip install."[^10] `pip install -r requirements.txt` reads one.[^11] Each line is a requirement specifier, such as `docopt == 0.6.1`; see [[version-constraints]].[^10] pip finds the dependencies of a package from "the project metadata (typically in pyproject.toml or setup.py ), not by discovering requirements.txt files embedded in projects".[^2]

**Pins and freeze.** `pip freeze` will "Output installed packages in requirements format".[^12] A requirements file with pinned versions from `pip freeze` pins "not only the top-level packages, but also all of their transitive dependencies".[^13] This is the lock file of a pip project; see [[pin-dependency-versions]] and [[lock-files]].

**Constraints files.** "Constraints files are requirements files that only control which version of a requirement is installed, not whether it is installed or not."[^2] Pass one with `-c constraints.txt`.

**Hash-checking mode.** This mode "uses local hashes, embedded in a requirements.txt file, to protect against remote tampering and network issues".[^9] Each requirement gets a `--hash` option, and each requirement "must be pinned".[^9] `--require-hashes` forces the mode; with `--only-binary :all:`, pip also refuses source distributions.[^9] The script shows the error for a wrong hash.

**The resolver.** pip chooses one version of each package. Since pip 20.3, "Pip’s dependency resolver is now capable of backtracking": when an assumption "is incorrect, it has to backtrack" and choose again.[^14] See [[dependency-resolution]].

## Common tasks

```text
python3 -m venv .venv                           # make a virtual environment
. .venv/bin/activate                            # use it (Windows: .venv\Scripts\activate)
python3 -m pip install requests                 # install a package and its dependencies
python3 -m pip install "requests>=2.31,<3"      # install with a version range
python3 -m pip install -r requirements.txt      # install from a requirements file
python3 -m pip install -r requirements.txt -c constraints.txt
python3 -m pip install -e .                     # install the project in editable mode
python3 -m pip install --upgrade requests       # upgrade a package
python3 -m pip uninstall requests               # remove a package
python3 -m pip freeze > requirements.txt        # pin what is installed
python3 -m pip list --outdated                  # list the packages that have newer versions
python3 -m pip show requests                    # show the metadata of a package
python3 -m pip check                            # check that the dependencies are compatible
python3 -m pip hash file.whl                    # print the hash of a file
python3 -m pip wheel . -w dist                  # build wheels
```

## Configuration

"Pip allows a user to change its behaviour via 3 mechanisms: command line options environment variables configuration files".[^15] On Unix, the user file is `$HOME/.config/pip/pip.conf`, the global file is `/etc/pip.conf`, and a virtual environment can have `$VIRTUAL_ENV/pip.conf`.[^15] On Windows, the global file is `C:\ProgramData\pip\pip.ini`.[^15] Each option of the command line has an environment variable of the form `PIP_<UPPER_LONG_NAME>`: "PIP_TIMEOUT=60 is the same as --timeout=60".[^15]

```text
# $HOME/.config/pip/pip.conf
[global]
index-url = https://pypi.example.com/simple/
timeout = 60

[install]
require-hashes = true
```

The script on this page sets `PIP_DISABLE_PIP_VERSION_CHECK` and `PIP_NO_CACHE_DIR` in this way.

## Alternatives

uv is another package and project manager for Python; see [[uv]]. Tools such as Poetry, PDM and Hatch manage a whole project; see [[python-packaging-tools]]. pip itself has a new command for lock files: pip 25.1 added "a new, experimental, pip lock command, implementing PEP 751", and pip 26.1 added "experimental support to read requirements from standardized pylock.toml files".[^4]

## History and versions

| Version | Date | Notes |
| --- | --- | --- |
| 0.2 | 2008-10-28 | The first release on PyPI[^16] |
| 1.0 | 2011-04-04 | Version 1.0[^16] |
| 20.3 | 2020-11-30 | The resolver backtracks[^14][^4] |
| 25.1 | 2025-04-26 | The experimental command `pip lock`[^4] |
| 26.1 | 2026-04-26 | Python 3.9 is no longer supported; `-r pylock.toml`[^4] |
| 26.3 | 2026-11-02 | The newest release on 2026-11-12[^4] |

## References
