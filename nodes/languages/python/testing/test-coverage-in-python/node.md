---
tag: AKYA4K
kind: feature
owner: python
slug: test-coverage-in-python
name: Test coverage in Python
aliases: [coverage.py, coverage run, coverage report, branch coverage, partial branch, pragma no cover, pytest-cov, trace module, sys.monitoring, line coverage]
area: languages/python/testing
level: core
summary: >-
  Coverage tools record which lines and branches a test run executed. coverage.py is the usual tool; the stdlib has the trace module and sys.monitoring.
grammar: null
spec: {standard: null, url: "https://coverage.readthedocs.io/en/latest/index.html", locator: "Quick start; Capabilities"}
see_also: [python/quality-control, python/unittest, python/testing-with-pytest, python/test-environments, python/performance-measurement]
sources:
  - url: https://coverage.readthedocs.io/en/latest/index.html
    title: Coverage.py documentation
    locator: Quick start; Capabilities
    accessed: 2026-11-16
  - url: https://coverage.readthedocs.io/en/latest/commands/cmd_run.html
    title: Execution, coverage.py documentation
    locator: Data file
    accessed: 2026-11-16
  - url: https://coverage.readthedocs.io/en/latest/branch.html
    title: Branch coverage measurement, coverage.py documentation
    locator: How to measure branch coverage; How it works
    accessed: 2026-11-16
  - url: https://coverage.readthedocs.io/en/latest/commands/cmd_report.html
    title: Coverage summary, coverage.py documentation
    locator: --fail-under; --show-missing
    accessed: 2026-11-16
  - url: https://coverage.readthedocs.io/en/latest/excluding.html
    title: Excluding code from coverage.py, coverage.py documentation
    locator: Excluding code; Advanced exclusion
    accessed: 2026-11-16
  - url: https://coverage.readthedocs.io/en/latest/config.html
    title: Configuration reference, coverage.py documentation
    locator: "[run] core; [report] exclude_also"
    accessed: 2026-11-16
  - url: https://pytest-cov.readthedocs.io/en/latest/readme.html
    title: Overview, pytest-cov documentation
    locator: Migrating from coverage.py
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/trace.html
    title: trace, Python 3.15 documentation
    locator: Command-line usage; Programmatic interface
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.monitoring.html
    title: sys.monitoring, Python 3.15 documentation
    locator: Tool identifiers; Events; Callbacks
    accessed: 2026-11-16
  - url: https://go.dev/doc/build-cover
    title: Coverage profiling support for integration tests, The Go Programming Language
    locator: Overview
    accessed: 2026-11-16
---

## Synopsis

```python norun reason=fragment group=grade file=grade.py
def grade(score):
    if score >= 90:
        return 'A'
    if score >= 50:
        return 'pass'
    return 'fail'
```

```python run group=grade file=main.py id=synopsis
import pathlib
import sys
import trace

tracer = trace.Trace(count=1, trace=0, ignoredirs=[sys.prefix, sys.exec_prefix])
tracer.run('import grade; grade.grade(95); grade.grade(70)')
tracer.results().write_results(show_missing=True, coverdir='cover')
report = next(pathlib.Path('cover').glob('*grade.cover'))
print(report.read_text(), end='')
```

```output for=synopsis
    1: def grade(score):
    2:     if score >= 90:
    1:         return 'A'
    1:     if score >= 50:
    1:         return 'pass'
>>>>>>     return 'fail'
```

Two "tests" call `grade` with 95 and 70. The stdlib `trace` module counts how often each line ran and marks the line that never ran with `>>>>>>`: no test gives a failing score. In a project, coverage.py does the same job for a whole test suite.

## Semantics

**r1.** Coverage.py "monitors your program, noting which parts of the code have been executed, then analyzes the source to identify code that could have been executed but was not."[^1] "Coverage measurement is typically used to gauge the effectiveness of tests."[^1]

**r2.** It is run in place of the interpreter: "If your test runner command starts with “python”, replace the initial “python” with “coverage run”."[^1] So `python -m unittest discover` becomes `coverage run -m unittest discover`, and `pytest` becomes `coverage run -m pytest`.[^1] The data goes to a file: "Coverage.py collects execution data in a file called “.coverage”."[^2] The `coverage report` and `coverage html` commands then report on it.[^3]

**r3.** "By default it will measure line (statement) coverage. It can also measure branch coverage."[^1] Branch coverage is turned on with `--branch`.[^3] "When measuring branches, coverage.py collects pairs of line numbers: a source and destination for each transition from one line to another." "Comparing the measured to the possible indicates missing branches."[^3] A line that did not reach every possible next line is "known as a partial branch."[^3]

**r4.** `coverage report -m` (`--show-missing`) shows "line numbers of statements in each module that weren't executed".[^4] With branch coverage on, "Missed branches are shown with an arrow: 40->45 means the branch from line 40 to line 45 is missing."[^4] `--fail-under=MIN` makes the command "Exit with a status of 2 if the total coverage is less than MIN", which lets a CI job fail on low coverage.[^4]

**r5.** Code can be left out of the report. "By default, any line with a comment of pragma: no cover is excluded. If that line introduces a clause, for example, an if clause, or a function or class definition, then the entire clause is also excluded."[^5] The `[report] exclude_also` setting adds more regular expressions "without overwriting the default set".[^6] Exclusion only affects reporting: "Excluded code is executed as usual, and its execution is recorded in the coverage data as usual."[^5]

**r6.** Coverage.py measures with one of several cores. The `[run] core` setting picks `ctrace`, "the C implementation of a sys.settrace function", `sysmon`, "the sys.monitoring implementation", or `pytrace`, the pure Python one.[^6] "The sysmon core does not yet support plugins, dynamic contexts, or some concurrency libraries."[^6]

**r7.** pytest-cov is a pytest plugin around coverage.py. Coverage.py's own docs say "Many people choose to use the pytest-cov plugin, but for most purposes, it is unnecessary."[^1] The plugin adds "Automatic erasing and combination of .coverage files and default reporting" and support for pytest-xdist.[^7]

**r8.** The stdlib `trace` module can count lines too. `--count` "shows how many times each statement was executed", and `--missing` will "mark lines which were not executed with >>>>>>".[^8] The same works from code: `Trace.runfunc` calls a function "under control of the Trace object", and `results()` returns a `CoverageResults` whose `write_results` writes the annotated files.[^8] The trace docs point to coverage.py as "A popular third-party coverage tool that provides HTML output along with advanced features such as branch coverage."[^8]

**r9.** `sys.monitoring` is the interpreter's hook for such tools: it "provides means to receive callbacks when events of interest occur."[^9] A tool claims an id with `use_tool_id`, and `COVERAGE_ID` is one of the ids "pre-defined to make co-operation of tools easier".[^9] A `LINE` callback is called as `func(code, line_number)`, and `DISABLE` is "A special value that can be returned from a callback function to disable events for the current code location."[^9] A line is then reported once, and later runs of it make no callback.

## Examples

### A coverage tool in a few lines

```python run id=monitoring
import sys

mon = sys.monitoring
TOOL = mon.COVERAGE_ID


def grade(score):
    if score >= 90:
        return 'A'
    if score >= 50:
        return 'pass'
    return 'fail'


seen = set()
calls = 0


def on_line(code, line_number):
    global calls
    calls += 1
    seen.add(line_number - code.co_firstlineno)
    return mon.DISABLE


mon.use_tool_id(TOOL, 'mini-cover')
mon.register_callback(TOOL, mon.events.LINE, on_line)
mon.set_local_events(TOOL, grade.__code__, mon.events.LINE)
for score in (95, 95, 70):
    grade(score)
mon.set_local_events(TOOL, grade.__code__, 0)
mon.free_tool_id(TOOL)

print('ran:', sorted(seen))
print('missed:', [n for n in range(1, 6) if n not in seen])
print('callbacks:', calls)
```

```output for=monitoring
ran: [1, 2, 3, 4]
missed: [5]
callbacks: 4
```

Line numbers are counted from the `def` line. Three calls ran four distinct lines, and the callback ran only four times, once per line, because it returns `DISABLE` (r9). Line 5, `return 'fail'`, never ran.

### Full line coverage, missing branch

```python run id=partial
import trace


def my_partial_fn(x):
    if x:
        y = 10
    return y


tracer = trace.Trace(count=1, trace=0)
tracer.runfunc(my_partial_fn, 1)
first = my_partial_fn.__code__.co_firstlineno
counts = tracer.results().counts
print({line - first: n for (path, line), n in sorted(counts.items())})
try:
    my_partial_fn(0)
except UnboundLocalError as e:
    print(type(e).__name__ + ':', e)
```

```output for=partial
{1: 1, 2: 1, 3: 1}
UnboundLocalError: cannot access local variable 'y' where it is not associated with a value
```

This is the function the coverage.py docs use for a partial branch.[^3] One call with `1` runs every line of the body, so line coverage is complete. The jump from `if x:` straight to `return y` never happened, and that path has a bug: with a false `x`, `y` is never set. `coverage run --branch` reports it as a missing branch (r3); line counts cannot.

### Starting measurement too late

```python run id=late
import pathlib
import sys
import tempfile
import trace

work = pathlib.Path(tempfile.mkdtemp())
(work / 'grade_late.py').write_text('''\
def grade(score):
    if score >= 90:
        return 'A'
    if score >= 50:
        return 'pass'
    return 'fail'
''')
sys.path.insert(0, str(work))

import grade_late  # imported before the tracer starts

tracer = trace.Trace(count=1, trace=0, ignoredirs=[sys.prefix, sys.exec_prefix])
tracer.runfunc(grade_late.grade, 95)
tracer.results().write_results(show_missing=True, coverdir=str(work / 'cover'))
print((work / 'cover' / 'grade_late.cover').read_text(), end='')
```

```output for=late
>>>>>> def grade(score):
    1:     if score >= 90:
    1:         return 'A'
>>>>>>     if score >= 50:
>>>>>>         return 'pass'
>>>>>>     return 'fail'
```

The program writes the same `grade` function to a module of its own. Here it is imported before the tracer starts, so the `def` line, which ran at import time, is marked as never run. The synopsis avoids this by doing the import inside `tracer.run`.

### Running a suite under coverage.py

coverage.py is not in the standard library (`pip install coverage`). With it installed, the commands for a unittest suite are:

```text
coverage run --branch -m unittest discover
coverage report -m --fail-under=90
coverage html
```

The settings can live in `pyproject.toml` instead of on the command line:

```toml
[tool.coverage.run]
branch = true
source = ["."]

[tool.coverage.report]
show_missing = true
fail_under = 90
exclude_also = [
    "if TYPE_CHECKING:",
]
```

The same steps, run on the `grade` module with two unittest tests. The main program calls coverage through `subprocess` and prints its report:

```python norun reason=fragment group=cov file=grade.py
def grade(score):
    if score >= 90:
        return 'A'
    if score >= 50:
        return 'pass'
    return 'fail'
```

```python norun reason=fragment group=cov file=test_grade.py
import unittest

from grade import grade


class GradeTest(unittest.TestCase):
    def test_top(self):
        self.assertEqual(grade(95), 'A')

    def test_pass(self):
        self.assertEqual(grade(70), 'pass')
```

```python run group=cov file=main.py id=coverage-py
import subprocess
import sys


def coverage(*args):
    done = subprocess.run([sys.executable, '-m', 'coverage', *args],
                          capture_output=True, text=True)
    print(done.stdout, end='')
    return done.returncode


coverage('run', '--branch', '--include=grade.py', '-m', 'unittest', '-q')
code = coverage('report', '-m', '--fail-under=90')
print('report exit code:', code)
```

```output for=coverage-py
Name       Stmts   Miss Branch BrPart  Cover   Missing
------------------------------------------------------
grade.py       6      1      4      1    80%   6
------------------------------------------------------
TOTAL          6      1      4      1    80%
Coverage failure: total of 80 is less than fail-under=90
report exit code: 2
```

The output is from coverage.py 7.17.3. Line 6, `return 'fail'`, is missing, and the BrPart column counts one partial branch: `if score >= 50:` never jumped to line 6 (r3, r4). The total is under the `--fail-under` limit, so `coverage report` exits with status 2, which fails a CI job (r4).

With pytest and pytest-cov, `pytest --cov=myproj tests/` measures the package while the tests run.[^7]

## Pitfalls

- Full coverage does not mean correct code. The partial example runs every line and still has a bug on the path it skipped. Turn on `--branch` (r3).
- Code that runs before measurement starts looks unexecuted, as the `def` line in the late example shows. With coverage.py, start the whole run with `coverage run` (r2) so that imports are measured too.
- `pragma: no cover` on a line that opens a clause excludes the whole clause (r5), which can hide more code than intended.
- `--source=.` matters for files that no test imports. The coverage.py docs say it limits measurement to the current directory "and also find files that weren’t executed at all".[^1] Without it, a module no test imports is not in the report.
- `coverage run -m pytest` and `pytest --cov` "do not necessarily import code the same way", per the pytest-cov docs.[^7] When switching, check that the tests still import the package.
- A branch tool built on `sys.monitoring` should not use the `BRANCH` event, which the docs list as deprecated. "Using BRANCH_LEFT and BRANCH_RIGHT events will give much better performance as they can be disabled independently."[^9] The examples here use only `LINE`.

## Comparison

Go builds coverage into its toolchain. "Go provides easy-to-use support for collecting coverage profiles at the level of package unit tests" through `go test -coverprofile`, and `go test -cover` reports a "statements covered" percentage.[^10] Python has no such flag in `unittest` or pytest; coverage comes from coverage.py, a separate tool (r1, r2). Go counts statements; coverage.py counts statements and, with `--branch`, branch destinations (r3).

[^1]: Coverage.py documentation.
[^3]: Branch coverage measurement, coverage.py documentation.
[^4]: Coverage summary, coverage.py documentation.
[^5]: Excluding code from coverage.py, coverage.py documentation.
[^6]: Configuration reference, coverage.py documentation.
[^7]: Overview, pytest-cov documentation.
[^8]: trace, Python 3.15 documentation.
[^9]: sys.monitoring, Python 3.15 documentation.
[^10]: Coverage profiling support for integration tests, The Go Programming Language.
[^2]: Execution, coverage.py documentation.
