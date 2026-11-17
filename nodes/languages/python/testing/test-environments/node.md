---
tag: HPDESW
kind: feature
owner: python
slug: test-environments
name: Test environments
aliases: [tox, nox, tox.toml, noxfile.py, nox.session, env_list, pass_env, test matrix, testing in a virtual environment, multiple Python versions]
area: languages/python/testing
level: core
summary: >-
  A test environment is a fresh virtual environment with only the declared packages and variables. tox and nox build one per Python version or task and run the tests in each.
grammar: null
spec: {standard: null, url: "https://tox.wiki/en/latest/explanation.html", locator: "Overview; System overview; Environment variable handling"}
see_also: [python/subprocess, python/environment-variables-from-python, python/unittest, python/testing-with-pytest, python/test-coverage-in-python, python/quality-control]
sources:
  - url: https://docs.python.org/3.15/library/venv.html
    title: venv, Python 3.15 documentation
    locator: Creating virtual environments; How venvs work
    accessed: 2026-11-16
  - url: https://tox.wiki/en/latest/explanation.html
    title: Concepts, tox documentation
    locator: Overview; System overview; Environment variable handling
    accessed: 2026-11-16
  - url: https://tox.wiki/en/latest/tutorial/getting-started.html
    title: Getting started, tox documentation
    locator: Core settings; Understanding the output
    accessed: 2026-11-16
  - url: https://nox.thea.codes/en/stable/index.html
    title: Welcome to Nox, Nox documentation
    locator: Welcome to Nox
    accessed: 2026-11-16
  - url: https://nox.thea.codes/en/stable/tutorial.html
    title: Tutorial, Nox documentation
    locator: Writing the configuration file; Running commands; Testing against different and multiple Pythons
    accessed: 2026-11-16
  - url: https://go.dev/doc/toolchain
    title: Go Toolchains, The Go Programming Language
    locator: Introduction; GOTOOLCHAIN
    accessed: 2026-11-16
---

## Synopsis

```python norun reason=fragment group=shapes file=test_shapes.py
import os
import unittest


def area(w, h):
    return w * h


class AreaTest(unittest.TestCase):
    def test_area(self):
        self.assertEqual(area(3, 4), 12)

    def test_mode(self):
        self.assertEqual(os.environ.get('APP_MODE', 'unset'), 'unset')
```

```python run group=shapes file=main.py id=synopsis
import os
import subprocess
import tempfile
import venv
from pathlib import Path

os.environ['APP_MODE'] = 'dev'

with tempfile.TemporaryDirectory() as tmp:
    env_dir = Path(tmp) / 'py'
    venv.create(env_dir, with_pip=False)
    python = env_dir / 'bin' / 'python'
    clean = {'PATH': f"{env_dir / 'bin'}{os.pathsep}{os.environ['PATH']}"}
    for name, env in [('inherited', None), ('clean', clean)]:
        result = subprocess.run(
            [python, '-m', 'unittest', 'test_shapes'],
            capture_output=True, text=True, env=env,
        )
        print(f'{name}: exit code {result.returncode},', result.stderr.splitlines()[-1])
```

```output for=synopsis
inherited: exit code 1, FAILED (failures=1)
clean: exit code 0, OK
```

This is what tox and nox do, by hand. The script makes a fresh virtual environment and runs the suite with its interpreter, twice. The first run inherits the shell's variables, and `test_mode` fails because `APP_MODE` leaks in. The second run gets only `PATH`, with the environment's `bin` first, so the result no longer depends on the machine.

## Semantics

**r1.** "The venv module supports creating lightweight “virtual environments”, each with their own independent set of Python packages installed in their site directories."[^1] By default an environment is isolated from the packages of its base Python, "so that only those explicitly installed in the virtual environment are available."[^1] Inside one, "It is sufficient to check sys.prefix != sys.base_prefix to determine if the current interpreter is running from a virtual environment."[^1]

**r2.** An environment need not be activated: "you can just specify the full path to that environment’s Python interpreter when invoking Python."[^1] The docs describe environments as not checked into source control and "Considered as disposable – it should be simple to delete and recreate it from scratch."[^1]

**r3.** "tox is an environment orchestrator."[^2] For each selected environment it creates the environment ("by default virtualenv is used"), installs the dependencies, optionally builds and installs the project, and runs the commands, then reports the results.[^2] "Only if all environments ran successfully tox will return exit code 0 (success)."[^2]

**r4.** The configuration lives in `tox.toml` (or the older `tox.ini`). "The env_list setting defines which environments run by default when you invoke tox without specifying any."[^3] The Python of an environment comes from its `base_python`, and if that is not set tox "will try to extract it from the environment name", so an environment named `3.13` runs on Python 3.13.[^2] "On the first run, tox creates virtual environments and installs dependencies." After that, "tox will automatically detect changes to your dependencies and recreate environments when needed."[^3] The `-r` flag forces a rebuild.[^2]

**r5.** "tox takes care of environment variable isolation. By default, it removes system environment variables not specified via pass_env and alters the PATH variable so that commands resolve within the current active tox environment."[^2] `set_env` values "are applied last and can override anything from the previous steps".[^2]

**r6.** "nox is a command-line tool that automates testing in multiple Python environments, similar to tox. Unlike tox, Nox uses a standard Python file for configuration."[^4] That file is `noxfile.py`, where "Sessions are declared using the @nox.session decorator."[^5] "The session.run function lets you run commands within the context of your session’s virtual environment."[^5]

**r7.** Passing a list as `python` to `@nox.session` runs the session on each interpreter; `nox --list` shows "this one session has been expanded into three distinct sessions" for three versions.[^5] Nox runs every session by default, and "You can use the --sessions argument (or -s ) to select which sessions to run."[^5]

## Examples

### A fresh environment has no extra packages

```python run id=fresh
import subprocess
import tempfile
import venv
from pathlib import Path

check = (
    'import importlib.util, sys;'
    'print(sys.prefix != sys.base_prefix, importlib.util.find_spec("pytest") is not None)'
)
with tempfile.TemporaryDirectory() as tmp:
    env_dir = Path(tmp) / 'fresh'
    venv.create(env_dir, with_pip=False)
    out = subprocess.run([env_dir / 'bin' / 'python', '-c', check], capture_output=True, text=True)
    print('in a venv, pytest found:', out.stdout.strip())
```

```output for=fresh
in a venv, pytest found: True False
```

The new interpreter knows it is in a virtual environment (r1), and pytest is not importable there even when the interpreter that made it has pytest. A test environment has to install the test tools it needs, which is what `deps` in tox and `session.install` in nox are for.

### tox.toml

The example configuration from the tox tutorial:[^3]

```toml
env_list = ["3.13", "3.12", "lint"]

[env_run_base]
description = "run the test suite with pytest"
deps = [
    "pytest>=8",
]
commands = [["pytest", { replace = "posargs", default = ["tests"], extend = true }]]

[env.lint]
description = "run linters"
skip_install = true
deps = ["ruff"]
commands = [["ruff", "check", { replace = "posargs", default = ["."], extend = true }]]
```

`tox` runs all three environments; `tox run -e 3.13,lint` runs two of them.[^3] `env_run_base` holds the settings shared by the test environments, and `skip_install` stops tox from installing the project into the lint environment.

### noxfile.py

```python norun reason=fragment group=nox file=noxfile.py
import nox


@nox.session(python=['3.12', '3.13'])
def tests(session):
    session.install('pytest')
    session.run('pytest')


@nox.session
def lint(session):
    session.install('ruff')
    session.run('ruff', 'check', '.')
```

```python run group=nox file=main.py id=nox-list
import os
import subprocess
import sys

done = subprocess.run([sys.executable, '-m', 'nox', '--list'],
                      capture_output=True, text=True)
print(done.stdout.replace(os.getcwd(), '<project>'), end='')
print('exit code:', done.returncode)
```

```output for=nox-list
Sessions defined in <project>/noxfile.py:

* tests-3.12
* tests-3.13
* lint

sessions marked with * are selected, sessions marked with - are skipped.
exit code: 0
```

The output is from nox 2026.10.20. `nox --list` reads the file without building any environment: the `python` list expands `tests` into `tests-3.12` and `tests-3.13`, and all three sessions are selected because nox runs every session by default. `nox -s lint` runs only the linter (r7). Because the file is Python, a session can use loops and conditions in place of tox's configuration keys.

## Pitfalls

- A suite that passes only on one machine often depends on that machine's state: an environment variable, as in the synopsis, or a package installed outside the environment. tox removes variables not listed in `pass_env` (r5); list the ones a test really needs.
- A version environment needs that interpreter on the machine. For an environment named `3.13`, tox "will use the virtualenv discovery logic" to find a Python 3.13.[^2] On a machine with only some of the versions, run a subset, such as `tox run -e 3.13` or `nox -s tests-3.13`; nox accepts the full session name "as displayed in the list".[^5]
- Reused environments save time but can be stale. tox recreates an environment when it detects a change in the dependencies (r4); after other changes, rebuild with `tox -r`.
- Running a tool by name inside a test (for example with `subprocess`) finds whatever is first on `PATH`. Both tox (r5) and the synopsis put the environment's `bin` first; or call the environment's interpreter by its full path (r2).
- Environment directories (`.tox`, `.nox`, `.venv`) are disposable (r2) and do not belong in version control.

## Comparison

Go chooses the toolchain, not a per-project interpreter environment. "The go command can use its bundled Go toolchain as well as other versions that it finds in the local PATH or downloads as needed," based on "the GOTOOLCHAIN environment setting and the go and toolchain lines in the main module’s go.mod file".[^6] "The GOTOOLCHAIN environment setting can force a specific Go version, overriding the go and toolchain lines," so one Go install can run the tests on another version.[^6] In Python the same job needs one environment per interpreter, which is what tox's `env_list` and nox's `python=` list set up (r4, r7).

[^1]: venv, Python 3.15 documentation.
[^2]: Concepts, tox documentation.
[^3]: Getting started, tox documentation.
[^4]: Welcome to Nox, Nox documentation.
[^5]: Tutorial, Nox documentation.
[^6]: Go Toolchains, The Go Programming Language.
