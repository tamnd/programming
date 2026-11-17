---
tag: WP3B6D
kind: feature
owner: python
slug: linters-and-formatters
name: Linters and Formatters
aliases: [linter, formatter, Ruff, ruff check, ruff format, Black, Flake8, Pyflakes, pycodestyle, noqa, PEP 8, code style checker, autoformatter]
area: languages/python/tooling
level: core
summary: >-
  Linters find likely bugs and style problems without running the code; formatters rewrite the layout. The page uses Ruff and Black, two third-party tools, with PEP 8 as the style they start from.
grammar: null
spec: {standard: "PEP 8", url: "https://peps.python.org/pep-0008/", locator: "Maximum Line Length"}
see_also: [python/coding-style, python/naming-conventions, python/compiled-python-files, smell-detection-tools, python-packaging-tools, python/the-ast-module]
sources:
  - url: https://peps.python.org/pep-0008/
    title: PEP 8, Style Guide for Python Code
    locator: Introduction; Maximum Line Length
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/linter/
    title: The Ruff Linter, Ruff documentation
    locator: ruff check; Rule selection; Fix safety; Error suppression
    accessed: 2026-11-18
  - url: https://black.readthedocs.io/en/stable/
    title: Black, The uncompromising code formatter
    locator: front page
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/formatter/
    title: The Ruff Formatter, Ruff documentation
    locator: ruff format; Philosophy; Conflicting lint rules
    accessed: 2026-11-18
  - url: https://black.readthedocs.io/en/stable/the_black_code_style/current_style.html
    title: The Black Code Style, Black documentation
    locator: Line length; Strings; The magic trailing comma
    accessed: 2026-11-18
  - url: https://pkg.go.dev/cmd/gofmt
    title: gofmt command, Go documentation
    locator: Overview; flags
    accessed: 2026-11-18
  - url: https://pkg.go.dev/cmd/vet
    title: vet command, Go documentation
    locator: Overview
    accessed: 2026-11-18
---

## Synopsis

```python run id=synopsis
import pathlib
import py_compile
import subprocess
import sys
import tempfile

source = '''\
import os
import sys


def check(value):
    result = value * 2
    if value == None:
        return missing
    return sys.argv
'''
with tempfile.TemporaryDirectory() as tmp:
    path = pathlib.Path(tmp, 'app.py')
    path.write_text(source)
    py_compile.compile(str(path), doraise=True)
    print('py_compile: no error')
    check = ['check', '--isolated', '--no-cache', '--output-format', 'concise']
    for args in (['--version'], [*check, 'app.py'], [*check, '--select', 'E711', 'app.py']):
        result = subprocess.run([sys.executable, '-m', 'ruff', *args], cwd=tmp,
                                capture_output=True, text=True)
        print(f'exit {result.returncode}:', *result.stdout.splitlines(), sep='\n  ')
```

```output for=synopsis
py_compile: no error
exit 0:
  ruff 0.17.5
exit 1:
  app.py:1:8: F401 [*] `os` imported but unused
  app.py:6:5: F841 Local variable `result` is assigned to but never used
  app.py:8:16: F821 Undefined name `missing`
  Found 3 errors.
  [*] 1 fixable with `--fix` (1 unsafe fix available with `--fix --unsafe-fixes`).
exit 1:
  app.py:7:17: E711 Comparison to `None` should be `cond is None`
  Found 1 error.
  1 unsafe fix available with `--fix --unsafe-fixes`.
```

The module compiles, so the interpreter sees no problem until `check` runs and reaches `missing`. Ruff reads the source and reports an unused import, an unused local variable and an undefined name, each with its file, line and column. `--isolated` makes Ruff ignore any configuration files, so the output shows the rules that are on with no configuration. The comparison `value == None` is not in that set; `--select E711` turns the rule on. The exit code is 1 when there are reports. The example needs the third-party package `ruff`, and the first line of the output shows the version that made it.

## Semantics

**r1.** PEP 8 is the base style. "One of Guido’s key insights is that code is read much more often than it is written. The guidelines provided here are intended to improve the readability of code and make it consistent across the wide spectrum of Python code."[^1] One rule is "Limit all lines to a maximum of 79 characters."[^1] A linter checks rules like these, and a formatter changes the layout to follow them.

**r2.** A linter reads code and does not run it. Ruff is "designed as a drop-in replacement for Flake8 (plus dozens of plugins), isort , pydocstyle , pyupgrade , autoflake , and more."[^2] "ruff check is the primary entrypoint to the Ruff linter. It accepts a list of files or directories, and lints all discovered Python files, optionally fixing any fixable errors."[^2]

**r3.** Each rule has a code. "Ruff's linter uses Flake8-style rule codes: an alphabetic prefix followed by digits (e.g., F401 ). The prefix indicates the source of the rule (e.g., F for Pyflakes, E for pycodestyle, ANN for flake8-annotations)."[^2] "Rule selectors like lint.select and lint.ignore accept either a full rule code (e.g., F401 ) or any valid prefix (e.g., F )."[^2]

**r4.** Some reports can be fixed by the tool. "Ruff labels fixes as "safe" and "unsafe". The meaning and intent of your code will be retained when applying safe fixes, but the meaning could change when applying unsafe fixes."[^2] In the synopsis, the unused import has a safe fix, marked `[*]`; the unused variable and the `None` comparison have only unsafe fixes.

**r5.** A report can be turned off on one line. "To ignore an individual violation, add # noqa: {code} to the end of the line".[^2] The docs give `x = 1  # noqa: F841` as an example.[^2]

**r6.** A formatter rewrites the layout and keeps the meaning. Black's front page says: "By using Black , you agree to cede control over minutiae of hand-formatting. In return, Black gives you speed, determinism, and freedom from pycodestyle nagging about formatting."[^3] The Ruff formatter "is designed as a drop-in replacement for Black".[^4] `ruff format --check` will "avoid writing any formatted files back, and instead exit with a non-zero status code upon detecting any unformatted files."[^4]

**r7.** Black has its own line length. "Black defaults to 88 characters per line, which happens to be 10% over 80."[^5] Black "prefers double quotes", and "A pre-existing trailing comma informs Black to always explode contents of the current bracket pair into one item per line."[^5]

## Examples

### Formatting with Black and checking with Ruff

```python run id=format
import os
import subprocess
import sys
import tempfile

source = "def area(w,h): return w*h\nsizes = {'small':(1,2), 'large':(3,4),}\nprint( area( *sizes['small'] ) )\n"
with tempfile.TemporaryDirectory() as tmp:
    env = dict(os.environ, BLACK_CACHE_DIR=tmp)
    black = [sys.executable, '-m', 'black']
    print(subprocess.run([*black, '--version'], env=env, capture_output=True, text=True).stdout.splitlines()[0])
    formatted = subprocess.run([*black, '-q', '-c', source], env=env, capture_output=True, text=True).stdout
    print(formatted, end='')
    for tool in (['black', '-q', '--check', '-'], ['ruff', 'format', '--isolated', '--no-cache', '--check', '-']):
        for label, text in (('original', source), ('formatted', formatted)):
            result = subprocess.run([sys.executable, '-m', *tool], input=text, cwd=tmp, env=env,
                                    capture_output=True, text=True)
            print(f'{tool[0]} --check {label}: exit {result.returncode}')
```

```output for=format
python -m black, 26.10.1 (compiled: yes)
def area(w, h):
    return w * h


sizes = {
    "small": (1, 2),
    "large": (3, 4),
}
print(area(*sizes["small"]))
black --check original: exit 1
black --check formatted: exit 0
ruff --check original: exit 1
ruff --check formatted: exit 0
```

`black -c` formats a string and prints the result. The function body moves to its own line, the operators and commas get spaces, and two blank lines separate the function from the next statement. The quotes become double quotes. The dictionary had a trailing comma after its last item, so Black puts one item on each line (r7). With `--check`, neither tool changes the code; each one gives exit code 1 for the original and 0 for Black's output, so here the two formatters agree. A `-` in place of a file name makes both tools read standard input.

## Pitfalls

- Code that compiles can still be wrong. `py_compile` found nothing in the synopsis module, but it has an undefined name that fails only when that line runs.
- Black's default line length is 88, but PEP 8 says 79 (r1, r7). A project that uses Black and also runs a pycodestyle line length rule must set the same length in both.
- A formatter and a linter can disagree. "the linter includes some rules that, when enabled, can cause conflicts with the formatter, leading to unexpected behavior."[^4]
- An unsafe fix can change what the code does (r4). Read the change before you keep it.
- A bare `# noqa` hides every report on the line. The docs label it "Ignore _all_ violations".[^2] Give the code, as in `# noqa: F841`.

## Comparison

Go has a formatter in its distribution. "Gofmt formats Go programs. It uses tabs for indentation and blanks for alignment."[^6] The `-d` flag is described as "Do not print reformatted sources to standard output. If a file's formatting is different than gofmt's, print diffs to standard output."[^6] Ruff and Black are third-party packages, so a Python project chooses its formatter and installs it. The Go linter is `go vet`: "Vet examines Go source code and reports suspicious constructs, such as Printf calls whose arguments do not align with the format string."[^7] Like a Python linter, "Vet uses heuristics that do not guarantee all reports are genuine problems, but it can find errors not caught by the compilers."[^7]

[^1]: PEP 8, Style Guide for Python Code.
[^2]: The Ruff Linter, Ruff documentation.
[^4]: The Ruff Formatter, Ruff documentation.
[^3]: Black, The uncompromising code formatter.
[^5]: The Black Code Style, Black documentation.
[^6]: gofmt command, Go documentation.
[^7]: vet command, Go documentation.
