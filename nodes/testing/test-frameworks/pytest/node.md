---
tag: YHMGX5
kind: tool
slug: pytest
name: pytest
aliases: [py.test]
area: testing/test-frameworks
level: core
summary: >-
  pytest is a test framework for Python. It collects plain test functions, explains a failed assert statement with the values on each side, and gives tests their fixtures through argument names.
role: [test-framework]
licence: mit
repo: https://github.com/pytest-dev/pytest
homepage: https://docs.pytest.org/
first_release: 2010-11-25
history:
  year: 2004
  people: [Holger Krekel]
  note: Holger Krekel started the work that became pytest in 2004, as part of the py library that grew out of the PyPy project. pytest 2.0.0, in November 2010, was the first release as a package of its own.
see_also: [test-framework, python-unittest, doctest-python, unit-testing, test-double, property-based-tests]
same_as:
  wikidata: Q28975377
  wikipedia: Pytest
sources:
  - url: https://docs.pytest.org/en/stable/index.html
    title: pytest documentation, home page
    locator: introduction; Features
    accessed: 2026-11-15
  - url: https://pypi.org/project/pytest/
    title: pytest on the Python Package Index
    locator: release history; meta data (licence and the Python versions)
    accessed: 2026-11-15
  - url: https://docs.pytest.org/en/stable/how-to/unittest.html
    title: pytest documentation, How to use unittest-based tests with pytest
    locator: introduction
    accessed: 2026-11-15
  - url: https://docs.pytest.org/en/stable/getting-started.html
    title: pytest documentation, Get Started
    locator: Install pytest; Create your first test; Run multiple tests; Group multiple tests in a class
    accessed: 2026-11-15
  - url: https://docs.pytest.org/en/stable/how-to/usage.html
    title: pytest documentation, How to invoke pytest
    locator: Specifying which tests to run; Other ways of calling pytest; Calling pytest from Python code
    accessed: 2026-11-15
  - url: https://docs.pytest.org/en/stable/reference/exit-codes.html
    title: pytest documentation, Exit codes
    locator: the whole page
    accessed: 2026-11-15
  - url: https://docs.pytest.org/en/stable/explanation/goodpractices.html
    title: pytest documentation, Good Integration Practices
    locator: Conventions for Python test discovery
    accessed: 2026-11-15
  - url: https://docs.pytest.org/en/stable/how-to/assert.html
    title: pytest documentation, How to write and report assertions in tests
    locator: Assertions about expected exceptions; Assertion introspection details
    accessed: 2026-11-15
  - url: https://docs.pytest.org/en/stable/how-to/fixtures.html
    title: pytest documentation, How to use fixtures
    locator: Requesting fixtures; Scope; Teardown/Cleanup; yield fixtures
    accessed: 2026-11-15
  - url: https://docs.pytest.org/en/stable/how-to/parametrize.html
    title: pytest documentation, How to parametrize fixtures and test functions
    locator: pytest.mark.parametrize, parametrizing test functions
    accessed: 2026-11-15
  - url: https://docs.pytest.org/en/stable/how-to/writing_plugins.html
    title: pytest documentation, Writing plugins
    locator: introduction; Plugin discovery order at tool startup
    accessed: 2026-11-15
  - url: https://docs.pytest.org/en/stable/history.html
    title: pytest documentation, History
    locator: the whole page
    accessed: 2026-11-15
  - url: https://docs.pytest.org/en/stable/how-to/doctest.html
    title: pytest documentation, How to run doctests
    locator: introduction
    accessed: 2026-11-15
  - url: https://docs.pytest.org/en/stable/reference/customize.html
    title: pytest documentation, Configuration
    locator: Configuration file formats; pytest.toml; pytest.ini; pyproject.toml; setup.cfg
    accessed: 2026-11-15
  - url: https://docs.pytest.org/en/stable/changelog.html
    title: pytest documentation, Changelog
    locator: pytest 9.2.0; pytest 9.0.0; pytest 8.0.0rc1; pytest 7.0.0; pytest 6.0.0
    accessed: 2026-11-15
---

## What it is

pytest is a [[test-framework]] for Python. Its documentation says that it "makes it easy to write small, readable tests, and can scale to support complex functional testing for applications and libraries."[^1] A pytest test is a plain function whose name starts with `test`, and the check is a plain `assert` statement. When the check is false, pytest shows the values that the expression had, so the test does not need special assertion methods.

pytest is not in the standard library. It is a package on the Python Package Index, under the MIT licence, and version 9.2.0 needs Python 3.10 or later.[^2] It also runs the tests of the standard library module [[python-unittest]]: "pytest supports running Python unittest -based tests out of the box."[^3]

## Install and first use

Install pytest in a virtual environment for the project, not in the Python of the operating system.[^4] With `venv` and `pip`:

```text
python3 -m venv .venv
source .venv/bin/activate
pip install -U pytest
pytest --version
```

In a project that uv manages, `uv add --dev pytest` adds pytest as a development dependency, and `uv run pytest` runs it.[^4]

A test file has a name such as `test_account.py`, and each test is a function that starts with `test`. In this file the second test is wrong on purpose: it expects 160 after deposits of 20 and 30.

```python norun reason=fragment group=first file=test_account.py
class Account:
    def __init__(self, balance):
        self.balance = balance

    def deposit(self, amount):
        self.balance += amount

    def __repr__(self):
        return f"Account(balance={self.balance})"


def test_deposit():
    account = Account(100)
    account.deposit(50)
    assert account.balance == 150


def test_two_deposits():
    account = Account(100)
    account.deposit(20)
    account.deposit(30)
    assert account.balance == 160
```

The usual command is `pytest` in the directory of the project. This page calls pytest from Python with `pytest.main`, which "acts as if you would call “pytest” from the command line" and returns the exit code.[^5] The option `-qq` makes the report short, `-rA` lists the outcome of each test at the end, and `-p no:cacheprovider` stops pytest from writing its cache directory.

```python run group=first file=main.py id=first-use
import pytest

code = pytest.main(["-qq", "-rA", "-p", "no:cacheprovider", "test_account.py"])
print("exit code:", int(code))
```

```output
.F                                                                       [100%]
=================================== FAILURES ===================================
______________________________ test_two_deposits _______________________________

    def test_two_deposits():
        account = Account(100)
        account.deposit(20)
        account.deposit(30)
>       assert account.balance == 160
E       assert 150 == 160
E        +  where 150 = Account(balance=150).balance

test_account.py:22: AssertionError
==================================== PASSES ====================================
=========================== short test summary info ============================
PASSED test_account.py::test_deposit
FAILED test_account.py::test_two_deposits - assert 150 == 160
exit code: 1
```

The first line has one character for each test: `.` for a pass and `F` for a failure. For the failure, pytest prints the source of the test, marks the line with `>`, and gives the values: the `assert` compared 150 with 160, and 150 came from `Account(balance=150).balance`. Exit code 1 means "Tests were collected and run but some of the tests failed".[^6]

## Concepts of the tool

**Discovery.** With no arguments, pytest starts from the current directory, or from the directories in the option `testpaths`. It goes into the subdirectories, and in them it looks for files that match `test_*.py` or `*_test.py`.[^7] From those files it collects "test prefixed test functions or methods outside of class", and the `test` methods of classes whose names start with `Test` and that have no `__init__` method.[^7] It also collects the subclasses of `unittest.TestCase`.[^7] Each test has a node id, such as `test_bank.py::test_withdraw[30-70]`, that names it on the command line.[^5]

**The plain assert.** pytest gets the values of a failed `assert` by a change to the code before it runs: "Reporting details about a failing assertion is achieved by rewriting assert statements before they are run."[^8] This applies only to the test modules: "pytest only rewrites test modules directly discovered by its test collection process, so asserts in supporting modules which are not themselves test modules will not be rewritten".[^8] To check that code raises an exception, a test uses `pytest.raises` as a context manager. Its argument `match` is a regular expression that pytest applies to the text of the exception with `re.search()`.[^8]

**Fixtures.** A fixture is a function with the decorator `@pytest.fixture`. A test asks for a fixture by name: "test functions request fixtures they require by declaring them as arguments."[^9] pytest finds the fixture with the same name as the parameter, runs it, and passes its result to the test.[^9] A fixture can request other fixtures in the same way. A fixture that uses `yield` in place of `return` also cleans up: "Any teardown code for that fixture is placed after the yield ."[^9] Each fixture has a scope, and the "Possible values for scope are: function , class , module , package or session ."[^9] The default scope, `function`, makes a new value for each test.[^9] Fixtures in a file named `conftest.py` are available to all the tests in its directory.[^9]

**Parametrize.** "The builtin pytest.mark.parametrize decorator enables parametrization of arguments for a test function."[^10] Each set of values is a separate test, with the values in the node id.

The following example has a module, a `conftest.py` with a fixture, and a test file that uses the fixture, a parametrized test and `pytest.raises`.

```python norun reason=fragment group=bank file=account.py
class InsufficientFunds(Exception):
    pass


class Account:
    def __init__(self, balance):
        self.balance = balance

    def deposit(self, amount):
        self.balance += amount

    def withdraw(self, amount):
        if amount > self.balance:
            raise InsufficientFunds(f"balance {self.balance}, asked {amount}")
        self.balance -= amount
```

```python norun reason=fragment group=bank file=conftest.py
import pytest

from account import Account


@pytest.fixture
def account():
    # The scope is "function", so each test gets a new account.
    acct = Account(100)
    yield acct
    # Teardown code goes after the yield; this fixture has nothing to clean up.
```

```python norun reason=fragment group=bank file=test_bank.py
import pytest

from account import InsufficientFunds


def test_deposit(account):
    account.deposit(50)
    assert account.balance == 150


@pytest.mark.parametrize("amount, rest", [(30, 70), (100, 0)])
def test_withdraw(account, amount, rest):
    account.withdraw(amount)
    assert account.balance == rest


def test_withdraw_more_than_balance(account):
    with pytest.raises(InsufficientFunds, match="asked 150"):
        account.withdraw(150)
    assert account.balance == 100
```

```python run group=bank file=main.py id=bank
import pytest

code = pytest.main(["-qq", "-rA", "-p", "no:cacheprovider", "test_bank.py"])
print("exit code:", int(code))
```

```output
....                                                                     [100%]
==================================== PASSES ====================================
=========================== short test summary info ============================
PASSED test_bank.py::test_deposit
PASSED test_bank.py::test_withdraw[30-70]
PASSED test_bank.py::test_withdraw[100-0]
PASSED test_bank.py::test_withdraw_more_than_balance
exit code: 0
```

`test_withdraw` runs twice, once for each pair of values, and the node id of each run has the values in brackets. Each test gets its own account from the fixture, so the deposit in `test_deposit` does not change the balance that `test_withdraw` sees.

**Plugins and hooks.** Most of pytest is made of plugins. The documentation says: "pytest implements all aspects of configuration, collection, running and reporting by calling well specified hooks of the following plugins:" the builtin plugins, the external plugins that are installed packages, and the `conftest.py` plugins.[^11] A hook is a function with a name that starts with `pytest_`.[^11] An installed plugin package registers itself through an entry point in its packaging metadata, so `pip install` is enough to turn it on.[^11]

**Exit codes.** The exit code tells a build script what happened.[^6]

| Code | Meaning |
|---|---|
| 0 | All tests were collected and passed |
| 1 | Tests ran, and some of them failed |
| 2 | The user interrupted the run |
| 3 | An internal error, or a plugin raised an exception while it was imported |
| 4 | A usage error on the command line, a plugin that pytest cannot find, or a `conftest.py` that fails to import |
| 5 | No tests were collected |
| 6 | The maximum number of warnings was exceeded |

The codes are in the enum `pytest.ExitCode`.[^6]

## Common tasks

Select the tests to run by file, by directory, by node id or by a keyword expression:[^5]

```text
pytest test_bank.py
pytest tests/
pytest test_bank.py::test_withdraw
pytest "test_bank.py::test_withdraw[30-70]"
pytest -k "withdraw and not more"
```

The option `-k` runs the tests whose keywords match the expression, and the keywords of a test include its name and the names of its file and class.[^5] `python -m pytest` is almost the same as the command `pytest`, but it also adds the current directory to `sys.path`.[^5]

Other common options:

- `-q`, or `--quiet`, makes the report shorter.[^4]
- `-x`, or `--exitfirst`, stops the run at the first failure. The option has been in pytest since 2004, in the py library.[^12]
- `--doctest-modules` also runs the examples in the docstrings of the modules, and pytest collects files that match `test*.txt` as doctest files by default.[^13] See [[doctest-python]].

To run an existing suite of `unittest` tests, run `pytest tests`. pytest collects the `unittest.TestCase` subclasses, and it supports `setUp`, `tearDown`, the `skip` decorators and, since version 9.0, `subTest`.[^3]

## Configuration

pytest reads its settings from one configuration file. Since version 9.0, the preferred format is TOML in `pytest.toml`, or in the table `[tool.pytest]` of `pyproject.toml`:[^14]

```toml
# pyproject.toml
[tool.pytest]
minversion = "9.0"
addopts = ["-ra", "-q"]
testpaths = ["tests", "integration"]
```

`addopts` adds options to each run, and `testpaths` gives the directories in which discovery starts.[^14] The table `[tool.pytest.ini_options]` in `pyproject.toml` is the older form, "supported since pytest 6.0", in which all values are strings.[^14][^15] pytest also reads `pytest.ini`, `tox.ini` and `setup.cfg`. The order of precedence is in the documentation: "pytest.toml files take precedence over other files, even when empty", and pytest.ini files come next.[^14] For `setup.cfg`, the documentation says that its use "is not recommended unless for very simple use cases".[^14]

pytest finds a rootdir from the arguments and the configuration files, and it uses the rootdir to build the node ids of the tests.[^14]

## Alternatives

**unittest** is in the standard library, so it needs no install. Tests are methods of a `unittest.TestCase` subclass and use methods such as `assertEqual`. pytest can run these tests too, so a project can change to pytest one file at a time.[^3] See [[python-unittest]].

**doctest** is also in the standard library. It checks the examples in documentation, not separate test functions. pytest can run doctests with `--doctest-modules`.[^13] See [[doctest-python]].

Libraries for other kinds of tests, such as the libraries for [[property-based-tests]], usually add to a framework like pytest and do not replace it.

## History and versions

The history page of pytest says that "pytest originally was born as part of PyPy , to make it easier to write tests for it."[^12] In 2004, Holger Krekel started a project first called std, later the py library. One of its aims was to use the plain `assert` statement in place of a set of `assertXYZ` methods, and to give "detailed information about underlying values" when an assertion fails.[^12] Its test tool was `py.test`. The page also says that "it’s difficult to answer the question when pytest was started", and that "One possible interpretation is to pick Europython 2004", which was around June or July 2004.[^12] In August 2009, py 1.0.0 added funcargs, the first form of fixtures, and a plugin architecture.[^12]

| Version | Date | Notes |
|---|---|---|
| 2.0.0 | 2010-11-25 | The first release as a package separate from py, with the command still called `py.test`[^12][^2] |
| 3.0.0 | 2016-08-19 | `pytest`, not `py.test`, becomes the recommended command[^12][^2] |
| 6.0.0 | 2020-07-28 | Configuration in `pyproject.toml`, in `[tool.pytest.ini_options]`[^14][^15] |
| 8.0.0 | 2024-01-27 | Support for Python 3.7 is dropped[^15] |
| 9.0.0 | 2025-11-05 | Subtests, native TOML configuration, a strict mode; support for Python 3.9 is dropped[^15] |
| 9.2.0 | 2026-10-27 | The arguments `indirect`, `ids` and `scope` of `@pytest.mark.parametrize` become keyword-only[^15] |

## References
