---
tag: QZSB65
kind: feature
owner: python
slug: project-layout
name: Project layout
aliases: [src layout, flat layout, Python project structure, pyproject.toml layout]
area: languages/python/getting-started
level: intro
summary: >-
  A Python project keeps pyproject.toml at its root, its import package either at the root (flat layout) or under src/ (src layout), the tests in tests/, and its virtual environment in .venv.
grammar: "project/ { pyproject.toml, [src/] package/ { __init__.py, __main__.py, ... }, tests/, .venv/ } (Python Packaging User Guide, src layout vs flat layout)"
spec: {standard: null, url: "https://packaging.python.org/en/latest/specifications/pyproject-toml/", locator: "pyproject.toml specification"}
see_also: [python/running-a-python-program, python/install-python, python/argument-passing]
sources:
  - url: https://docs.python.org/3.15/tutorial/modules.html#packages
    title: The Python Tutorial, Python 3.15
    locator: 6.4 Packages
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/tutorials/packaging-projects/
    title: Packaging Python Projects, Python Packaging User Guide
    locator: A simple project; Creating the package files; Creating a test directory
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/discussions/src-layout-vs-flat-layout/
    title: src layout vs flat layout, Python Packaging User Guide
    locator: src layout vs flat layout
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/specifications/pyproject-toml/
    title: pyproject.toml specification, Python Packaging User Guide
    locator: pyproject.toml specification; requires-python; Entry points
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/guides/writing-pyproject-toml/
    title: Writing your pyproject.toml, Python Packaging User Guide
    locator: Writing your pyproject.toml; requires-python; Creating executable scripts
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/__main__.html#main-py-in-python-packages
    title: __main__, Top-level code environment, Python 3.15 documentation
    locator: __main__.py in Python Packages; Idiomatic Usage
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/unittest.html#test-discovery
    title: unittest, Python 3.15 documentation
    locator: Test Discovery
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/venv.html
    title: venv, Python 3.15 documentation
    locator: introduction; Creating virtual environments
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/venv.html#creating-virtual-environments
    title: The Python Tutorial, Python 3.15
    locator: 12.2 Creating Virtual Environments
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/tomllib.html
    title: tomllib, Python 3.15 documentation
    locator: tomllib.load, tomllib.loads
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/unittest.html#unittest.main
    title: unittest, Python 3.15 documentation
    locator: unittest.main
    accessed: 2026-11-16
---

## Synopsis

```text
flat layout                       src layout
project/                          project/
├── pyproject.toml                ├── pyproject.toml
├── README.md                     ├── README.md
├── greet/                        ├── src/
│   ├── __init__.py               │   └── greet/
│   ├── __main__.py               │       ├── __init__.py
│   └── core.py                   │       ├── __main__.py
├── tests/                        │       └── core.py
│   ├── __init__.py               ├── tests/
│   └── test_core.py              │   ├── __init__.py
└── .venv/                        │   └── test_core.py
                                  └── .venv/
```

```toml
[build-system]
requires = ["hatchling >= 1.26"]
build-backend = "hatchling.build"

[project]
name = "greet"
version = "0.1.0"
requires-python = ">= 3.10"

[project.scripts]
greet = "greet.__main__:main"
```

## Semantics

**r1.** A directory with an `__init__.py` file is a regular package, and the files in it are its modules. The file can be empty.[^1] The project usually has one import package, named like the project.[^2]

**r2.** In the flat layout, the import package sits at the top of the project, next to the configuration files. In the src layout, it sits in a subdirectory, "typically named src/".[^3] Example: [`flat-layout`](#flat-layout), [`src-layout`](#src-layout).

**r3.** The src layout "requires installation of the project to be able to run its code, and the flat layout does not." The reason is the import path: the interpreter puts the current directory first on it, so in the flat layout the copy in the project directory hides an installed copy. The src layout keeps the package out of the root directory, so the tests import the installed copy.[^3] Example: [`src-layout`](#src-layout).

**r4.** `pyproject.toml` is a TOML file at the root of the project. It holds the configuration of the packaging tools and of other tools. The specification defines four tables: `[build-system]`, `[project]`, `[dependency-groups]` and `[tool]`.[^4] The guide says that the `[build-system]` table "should always be present"; it names the build backend.[^5] Example: [`read-pyproject`](#read-pyproject).

**r5.** In the `[project]` table, `name` is required, `requires-python` gives the Python versions that the project supports, and `[project.scripts]` makes a command. After `pip install`, the command `spam-cli = "spam:main_cli"` does the equivalent of `import sys; from spam import main_cli; sys.exit(main_cli())`.[^5] [^4]

**r6.** A file `__main__.py` in a package runs when the package is started with `python -m package`. It usually provides the command line interface of the package, it stays short, and it imports the functions that it calls from the other modules.[^6] Example: [`flat-layout`](#flat-layout).

**r7.** The tests go in a directory `tests/` at the root.[^2] `python -m unittest` is short for `python -m unittest discover`. It starts in the current directory and imports the files that match `test*.py`, so the file names must be valid identifiers. It does not search a subdirectory that has no `__init__.py`.[^7] Example: [`flat-layout`](#flat-layout), [`tests-no-init`](#tests-no-init).

**r8.** The virtual environment of the project goes in a directory that is by convention named `.venv` or `venv`, in the project directory, or in a shared directory such as `~/.virtualenvs`. It is not checked into source control, and it holds no project code: it can be deleted and made again.[^8] The tutorial prefers `.venv`, because the name is hidden in the shell and does not clash with `.env` files.[^9]

## Examples

The examples build a small project in a temporary directory and run it in a child interpreter, as a person would from the root of the project.

The flat layout runs from the project root with no install (rules r2, r6 and r7). `python -m greet` runs `greet/__main__.py`, and `python -m unittest` finds the tests.

```python run id=flat-layout
import os
import subprocess
import sys
import tempfile

FILES = {
    "pyproject.toml": '[project]\nname = "greet"\nversion = "0.1.0"\n',
    "greet/__init__.py": "",
    "greet/core.py": "def hello(name):\n    return f'Hello, {name}!'\n",
    "greet/__main__.py": "import sys\nfrom greet.core import hello\nprint(hello(sys.argv[1]))\n",
    "tests/__init__.py": "",
    "tests/test_core.py": (
        "import unittest\nfrom greet.core import hello\n\n"
        "class TestHello(unittest.TestCase):\n"
        "    def test_hello(self):\n"
        "        self.assertEqual(hello('Ada'), 'Hello, Ada!')\n"
    ),
}

with tempfile.TemporaryDirectory() as root:
    for path, text in FILES.items():
        os.makedirs(os.path.join(root, os.path.dirname(path)), exist_ok=True)
        with open(os.path.join(root, path), "w") as f:
            f.write(text)
    run = subprocess.run([sys.executable, "-m", "greet", "Ada"], capture_output=True, text=True, cwd=root)
    test = subprocess.run([sys.executable, "-m", "unittest"], capture_output=True, text=True, cwd=root)
print("python -m greet Ada:", run.stdout.strip())
print("python -m unittest: ", test.stderr.strip().splitlines()[-1], "status", test.returncode)
```

```output for=flat-layout
python -m greet Ada: Hello, Ada!
python -m unittest:  OK status 0
```

The same package under `src/` is not found from the project root (rule r3). An editable install (`python -m pip install -e .`) is the usual fix. The example uses `PYTHONPATH=src` in its place, to stay offline.

```python run id=src-layout
import os
import subprocess
import sys
import tempfile

with tempfile.TemporaryDirectory() as root:
    os.makedirs(os.path.join(root, "src", "greet"))
    with open(os.path.join(root, "src", "greet", "__init__.py"), "w") as f:
        f.write("")
    with open(os.path.join(root, "src", "greet", "__main__.py"), "w") as f:
        f.write("print('Hello from src/greet')\n")
    plain = subprocess.run([sys.executable, "-m", "greet"], capture_output=True, text=True, cwd=root)
    env = dict(os.environ, PYTHONPATH="src")
    with_path = subprocess.run([sys.executable, "-m", "greet"], capture_output=True, text=True, cwd=root, env=env)
print("no install:   ", plain.stderr.strip().split(": ")[-1])
print("PYTHONPATH=src:", with_path.stdout.strip())
```

```output for=src-layout
no install:    No module named greet
PYTHONPATH=src: Hello from src/greet
```

Tools read `pyproject.toml` as TOML (rule r4). The standard module `tomllib` parses it into a dictionary.[^10]

```python run id=read-pyproject since=3.11
import tomllib

text = """
[build-system]
requires = ["hatchling >= 1.26"]
build-backend = "hatchling.build"

[project]
name = "greet"
version = "0.1.0"
requires-python = ">= 3.10"

[project.scripts]
greet = "greet.__main__:main"
"""
config = tomllib.loads(text)
print(sorted(config))
print(config["project"]["requires-python"])
print(config["project"]["scripts"])
```

```output for=read-pyproject
['build-system', 'project']
>= 3.10
{'greet': 'greet.__main__:main'}
```

## Versions

> [!since v="3.11"]
> The standard library can read TOML with `tomllib`. The example `read-pyproject` needs it.[^10]

Test discovery and namespace packages changed twice. The 3.11 release required an `__init__.py` in the start directory and its subdirectories. The 3.14 release accepts a namespace package as the start directory again, but it still skips a subdirectory with no `__init__.py`.[^7]

## Pitfalls

### A `tests` directory with no `__init__.py`

`python -m unittest` from the root skips a directory with no `__init__.py` (rule r7). It then runs no test, and it exits with status 5, which means that no tests ran.[^7] [^11]

```python wrong id=tests-no-init
import os
import subprocess
import sys
import tempfile

with tempfile.TemporaryDirectory() as root:
    os.makedirs(os.path.join(root, "tests"))
    with open(os.path.join(root, "tests", "test_math.py"), "w") as f:
        f.write("import unittest\n\nclass T(unittest.TestCase):\n    def test_add(self):\n        self.assertEqual(1 + 1, 2)\n")
    result = subprocess.run([sys.executable, "-m", "unittest"], capture_output=True, text=True, cwd=root)
print(result.stderr.strip().splitlines()[-1], "status", result.returncode)
```

```output for=tests-no-init
NO TESTS RAN status 5
```

Add an empty `tests/__init__.py`, or run `python -m unittest discover -s tests`.

### The flat layout can test the wrong copy

In the flat layout, the package in the current directory comes first on the import path. The tests then run against the files in the project, even when the installed copy lacks some of them, for example because the packaging configuration leaves a file out (rule r3).[^3]

### The virtual environment in the package or in Git

`.venv` belongs at the root of the project, not inside `src/` or the package, and not in source control (rule r8).[^8] Some tools look for it there. The `venv` module of 3.13 and later writes a `.gitignore` into the new environment, so Git ignores it by default.[^8]

## Comparison

The packaging guide notes that some tools that build the package also create the project. This page uses only the layout and the files that the guides describe; the layout of a project of another language is on the page of that language.[^2]
