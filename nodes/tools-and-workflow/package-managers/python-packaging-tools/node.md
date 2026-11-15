---
tag: 6WWF6N
kind: tool
slug: python-packaging-tools
name: Python packaging tools
aliases: [pyproject.toml, setuptools, Hatch, Hatchling, Poetry, PDM, conda, build backend, build frontend, PEP 517, PEP 621, pylock.toml]
area: tools-and-workflow/package-managers
level: advanced
summary: >-
  Python packaging uses pyproject.toml, build frontends and build backends. This page shows setuptools, Hatch, Poetry, PDM and conda, and how a frontend builds a wheel.
role: [build, package-manager]
licence: mit
repo: https://github.com/pypa/setuptools
homepage: https://packaging.python.org/
see_also: [pip-tool, uv, package-manager-concept, lock-files, publish-a-package, package-a-program-for-release, version-constraints, add-a-dependency]
sources:
  - url: https://packaging.python.org/en/latest/guides/tool-recommendations/
    title: Python Packaging User Guide, Tool recommendations
    locator: the introduction; Installing packages; Lock files; Build backends
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/guides/writing-pyproject-toml/
    title: Python Packaging User Guide, Writing your pyproject.toml
    locator: the introduction; Declaring the build backend
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0517/
    title: PEP 517, A build-system independent format for source trees
    locator: the header; Abstract; Terminology and goals
    accessed: 2026-11-16
  - url: https://hatch.pypa.io/latest/
    title: Hatch documentation, the home page
    locator: the introduction
    accessed: 2026-11-16
  - url: https://python-poetry.org/docs/
    title: Poetry documentation, Introduction
    locator: the introduction; System requirements; Installation
    accessed: 2026-11-16
  - url: https://pdm-project.org/en/latest/
    title: PDM documentation, Introduction
    locator: Feature highlights; Installation
    accessed: 2026-11-16
  - url: https://python-poetry.org/docs/basic-usage/
    title: Poetry documentation, Basic usage
    locator: Project setup; Installing dependencies
    accessed: 2026-11-16
  - url: https://pdm-project.org/en/latest/usage/project/
    title: PDM documentation, New project
    locator: pdm init; Choose a Python interpreter
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0518/
    title: PEP 518, Specifying Minimum Build System Requirements for Python Projects
    locator: the header; Abstract; build-system table
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/tutorials/packaging-projects/
    title: Python Packaging User Guide, Packaging Python Projects
    locator: Generating distribution archives; Uploading the distribution archives
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0621/
    title: PEP 621, Storing project metadata in pyproject.toml
    locator: the header; Abstract
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0751/
    title: PEP 751, A file format to record Python dependencies for installation reproducibility
    locator: the header; Abstract
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/specifications/pylock-toml/
    title: Python Packaging User Guide, pylock.toml Specification
    locator: File Name
    accessed: 2026-11-16
  - url: https://hatch.pypa.io/latest/intro/
    title: Hatch documentation, Introduction
    locator: New project
    accessed: 2026-11-16
  - url: https://docs.conda.io/projects/conda/en/latest/index.html
    title: conda documentation, the home page
    locator: the introduction
    accessed: 2026-11-16
  - url: https://docs.conda.io/projects/conda/en/latest/user-guide/tasks/manage-environments.html
    title: conda documentation, Managing environments
    locator: Creating an environment with commands; Creating an environment from an environment.yml file
    accessed: 2026-11-16
  - url: https://setuptools.pypa.io/en/latest/userguide/pyproject_config.html
    title: setuptools documentation, Configuring setuptools using pyproject.toml files
    locator: the introduction
    accessed: 2026-11-16
  - url: https://docs.conda.io/projects/conda/en/latest/user-guide/concepts/packages.html
    title: conda documentation, Packages
    locator: What is a package?
    accessed: 2026-11-16
  - url: https://pypi.org/pypi/poetry/json
    title: PyPI, the JSON metadata of poetry
    locator: info; releases
    accessed: 2026-11-16
  - url: https://pypi.org/pypi/setuptools/json
    title: PyPI, the JSON metadata of setuptools
    locator: info.license_expression; releases
    accessed: 2026-11-16
  - url: https://pypi.org/pypi/hatch/json
    title: PyPI, the JSON metadata of hatch
    locator: info; releases
    accessed: 2026-11-16
  - url: https://pypi.org/pypi/pdm/json
    title: PyPI, the JSON metadata of pdm
    locator: info; releases
    accessed: 2026-11-16
---

## What it is

"The Python packaging landscape consists of many different tools."[^1] They do three jobs:

- **Describe the project.** The file `pyproject.toml` holds the name, the version and the dependencies of the project, and it names the build backend.[^2]
- **Build the package.** A build frontend reads `pyproject.toml` and calls a build backend, which makes the wheel and the source distribution.[^3] Common backends are setuptools, Hatchling, Flit-core, PDM-backend and Poetry-core.[^1]
- **Manage the project.** Hatch, Poetry and PDM are project managers. They create projects, manage dependencies or environments, and build packages.[^4][^5][^6] Poetry and PDM also write a lock file.[^7][^8]

pip installs packages and uv is a fast tool for the whole workflow; see [[pip-tool]] and [[uv]]. conda is a separate system with its own package format; see the section Alternatives.

## Install and first use

You do not install the build backend by hand. The `requires` key of the `[build-system]` table lists the packages "required to execute the build system".[^9] "One of the responsibilities of a build frontend is to set up the Python environment in which the build backend will run."[^3] The packaging tutorial builds a project with the frontend `build`:[^10]

```text
python3 -m pip install --upgrade build
python3 -m build              # writes dist/*.whl and dist/*.tar.gz
```

"The tar.gz file is a source distribution whereas the .whl file is a built distribution."[^10]

This script makes a project with a `pyproject.toml` for setuptools and builds a wheel. "In a command like pip wheel some-directory/ , pip is acting as a build frontend."[^3] The run uses the setuptools that is already installed (`--no-build-isolation`), so it needs no network. The output comes from setuptools 68.1.2 and pip 24.0 on Python 3.12.

```bash run timeout=30 id=pypkg-first
d=$(mktemp -d)
cd "$d"
export PIP_CACHE_DIR="$d/pip-cache" PIP_NO_INDEX=1 PIP_DISABLE_PIP_VERSION_CHECK=1
py=/usr/bin/python3
mkdir -p greet/src/greet
cat > greet/pyproject.toml <<'TOML'
[build-system]
requires = ["setuptools >= 61"]
build-backend = "setuptools.build_meta"

[project]
name = "greet"
version = "1.0.0"
description = "Say hello"
requires-python = ">= 3.9"
dependencies = []

[project.scripts]
greet = "greet:main"
TOML
printf 'def main():\n    print("Hello from greet")\n' > greet/src/greet/__init__.py
# A build frontend (pip) calls the build backend (setuptools) to make a wheel.
"$py" -m pip wheel ./greet --no-deps --no-build-isolation -w dist -q 2>&1 | grep -v -i 'warning'
ls dist
w=$(ls dist/*.whl)
"$py" -m zipfile -l "$w" | awk 'NR > 1 { print "  " $1 }'
"$py" -c 'import sys, zipfile
z = zipfile.ZipFile(sys.argv[1])
meta = z.read("greet-1.0.0.dist-info/METADATA").decode()
for line in meta.splitlines():
    if line.split(":")[0] in ("Name", "Version", "Summary", "Requires-Python"):
        print(line)
print(z.read("greet-1.0.0.dist-info/entry_points.txt").decode().strip())' "$w"
cd /
rm -rf "${d:?}"
```

```output for=pypkg-first
greet-1.0.0-py3-none-any.whl
  greet/__init__.py
  greet-1.0.0.dist-info/METADATA
  greet-1.0.0.dist-info/WHEEL
  greet-1.0.0.dist-info/entry_points.txt
  greet-1.0.0.dist-info/top_level.txt
  greet-1.0.0.dist-info/RECORD
Name: greet
Version: 1.0.0
Summary: Say hello
Requires-Python: >=3.9
[console_scripts]
greet = greet:main
```

The wheel holds the code and a `.dist-info` folder. The fields of the `[project]` table become the fields of `METADATA`, and `[project.scripts]` becomes `entry_points.txt`.

## Concepts of the tool

**pyproject.toml.** "There are four possible TOML tables in this file."[^2]

- "The [build-system] table is strongly recommended. It allows you to declare which build backend you use and which other dependencies are needed to build your project."[^2]
- "The [project] table is the format that most build backends use to specify your project’s basic metadata".[^2]
- "The [dependency-groups] table specifies dependencies for development tasks such as testing and linting."[^2]
- "The [tool] table has tool-specific subtables".[^2]

**Frontend and backend.** PEP 517 (created 2015-09-30) defines the interface between the two.[^3] "A build frontend is a tool that users might run that takes arbitrary source trees or source distributions and builds wheels from them. The actual building is done by each source tree’s build backend."[^3] PEP 518 (created 2016-05-10) added the `[build-system]` table, so a project can say "what build dependencies they have in order to execute their chosen build system".[^9] Before these PEPs, setuptools was in practice the only choice, because "distutils/setuptools provide the standard interface for installing packages expected by both users and installation tools like pip".[^3]

**Project metadata.** PEP 621 (created 2020-06-22) "specifies how to write a project’s core metadata in a pyproject.toml file for packaging-related tools to consume."[^11] The `[project]` table "is understood by most build backends".[^2] Poetry "before version 2.0 (released January 5, 2025) did not use the [project] table, it used the [tool.poetry] table instead. With version 2.0, it supports both."[^2]

**The backends.** The packaging guide gives the `[build-system]` table of each common backend:[^2]

| Backend | `requires` | `build-backend` |
|---|---|---|
| Hatchling | `"hatchling >= 1.26"` | `hatchling.build` |
| setuptools | `"setuptools >= 77.0.3"` | `setuptools.build_meta` |
| Flit | `"flit_core >= 3.12.0, <5"` | `flit_core.buildapi` |
| PDM | `"pdm-backend >= 2.4.0"` | `pdm.backend` |
| uv-build | `"uv_build >= 0.13, <0.14"` | `uv_build` |

Setuptools "used to be the only build backend".[^1] For extension modules, the guide lists setuptools, which "natively supports C and C++".[^1]

**Lock files.** "pip-tools and Pipenv are two recognized tools to create lock files".[^1] Poetry writes `poetry.lock`, and PDM writes `pdm.lock`.[^7][^8] PEP 751 (accepted 2025-03-31) "proposes a new file format for specifying dependencies to enable reproducible installation in a Python environment."[^12] The file is named `pylock.toml`, or `pylock.<name>.toml`.[^13] See [[lock-files]].

## Common tasks

**Hatch.** "Hatch is a modern, extensible Python project manager."[^4] `hatch new "Hatch Demo"` makes a project in `hatch-demo` with a `src` layout, tests and a `pyproject.toml`.[^14]

**Poetry.** "Poetry is a tool for dependency management and packaging in Python."[^5] "Poetry requires Python 3.10+."[^5]

```text
poetry new poetry-demo        # make a project
poetry add pendulum           # add a dependency
poetry install                # install the dependencies; writes poetry.lock
poetry run pytest             # run a command in the environment
```

When `poetry install` runs and there is no lock file, Poetry "writes all the packages and their exact versions that it downloaded to the poetry.lock file".[^7] The documentation says: "You should commit the poetry.lock file to your project repo".[^7]

**PDM.** PDM "is a modern Python package and dependency manager supporting the latest PEP standards."[^6] It has "A PEP 517 build backend" and uses "PEP 621 project metadata".[^6] `pdm init` creates a `pyproject.toml` for a project, and the interpreter that you choose is stored in `.pdm-python`.[^8]

**conda.** "Conda provides package, dependency, and environment management for any language."[^15]

```text
conda create -n myenv python=3.9      # make an environment
conda install -n myenv scipy          # add a package to it
conda create --file environment.yml   # make an environment from a file
```

The commands come from the conda documentation.[^16]

**Upload to PyPI.** The packaging tutorial uploads with `python3 -m twine upload`.[^10] See [[publish-a-package]].

## Configuration

The settings of each tool are in its `[tool]` subtable of `pyproject.toml`, for example `[tool.hatch]`.[^2] "Setuptools has adopted this standard and will use the information contained in this file as an input in the build process."[^17] setuptools still supports `setup.cfg` and `setup.py`. The guide recommends the `[project]` table for new projects, and `setup.py` "only if programmatic configuration is needed".[^1] It also says: "do not use direct python setup.py invocations."[^1]

## Alternatives

conda is not a Python-only tool. A conda package "is a compressed tarball file (.tar.bz2 ) or .conda file" that can contain "system-level libraries", Python modules and programs.[^18] The packaging guide says: "For scientific software specifically, consider conda or Spack."[^1]

uv manages projects and installs packages, and its build backend is `uv_build`; see [[uv]].[^2] pip is "the standard tool to install packages from PyPI"; see [[pip-tool]].[^1]

## History and versions

| Event | Date |
|---|---|
| PEP 517 created | 2015-09-30 |
| PEP 518 created | 2016-05-10 |
| PEP 621 created | 2020-06-22 |
| Poetry 2.0.0, with the `[project]` table | 2025-01-05 |
| PEP 751 (`pylock.toml`) accepted | 2025-03-31 |

The dates of the PEPs are from their headers.[^3][^9][^11][^12] The Poetry date is from PyPI.[^19] The latest versions at 2026-11-12, from PyPI, and their licences:

| Tool | Version | Released | Licence |
|---|---|---|---|
| setuptools | 84.2.1 | 2026-10-20 | MIT |
| Hatch | 1.18.2 | 2026-10-28 | MIT |
| Poetry | 2.5.1 | 2026-09-20 | MIT |
| PDM | 2.29.7 | 2026-11-06 | MIT |

The rows come from the PyPI metadata of each package.[^20][^21][^19][^22]

## References
