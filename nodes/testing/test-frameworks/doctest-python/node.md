---
tag: 45DB1H
kind: library
slug: doctest-python
name: Python doctest
aliases: [doctest]
area: testing/test-frameworks
level: core
summary: >-
  doctest is a module of the Python standard library that finds examples of interactive Python sessions in docstrings and text files, runs them, and checks that the output is the same as shown.
role: [test-framework]
licence: psf-2.0
repo: https://github.com/python/cpython/blob/main/Lib/doctest.py
homepage: https://docs.python.org/3/library/doctest.html
first_release: 2001-04-17
history:
  year: 2001
  people: [Tim Peters]
  note: Tim Peters released doctest to the public domain in January 2001, and it came into the standard library in Python 2.1, in April 2001. Jim Fulton and Edward Loper made major changes to it later.
see_also: [test-framework, python-unittest, pytest, unit-testing]
same_as:
  wikidata: Q5287267
  wikipedia: Doctest
sources:
  - url: https://docs.python.org/3/library/doctest.html
    title: The Python Standard Library, doctest, Test interactive Python examples
    locator: introduction; Simple Usage; Command-line Usage; How It Works; Option Flags; Directives; Warnings; Unittest API; Soapbox
    accessed: 2026-11-15
  - url: https://docs.pytest.org/en/stable/how-to/doctest.html
    title: pytest documentation, How to run doctests
    locator: introduction; --doctest-modules
    accessed: 2026-11-15
  - url: https://github.com/python/cpython/blob/main/Lib/doctest.py
    title: CPython source, Lib/doctest.py
    locator: the header comment
    accessed: 2026-11-15
  - url: https://docs.python.org/3/whatsnew/2.1.html
    title: A.M. Kuchling, What's New in Python 2.1
    locator: Introduction; New and Improved Modules
    accessed: 2026-11-15
---

## What it is

`doctest` is a module of the Python standard library that tests the examples in documentation. The documentation says: "The doctest module searches for pieces of text that look like interactive Python sessions, and then executes those sessions to verify that they work exactly as shown."[^1] An example is a line that starts with the prompt `>>>`, followed by the output that the interactive interpreter prints.

The documentation gives three common uses:[^1]

- To check that the examples in the docstrings of a module still work.
- To do regression tests with examples in a text file.
- To write a tutorial that is full of examples, so that the text is "executable documentation".

`doctest` is a small [[test-framework]] of its own. It finds the examples, runs them, compares the output and reports the examples that fail. It also makes `unittest` suites, so its examples can run with [[python-unittest]] or [[pytest]].

## Install and first use

`doctest` comes with Python, so there is nothing to install. Write examples in the docstrings, as they would look in the interactive interpreter, and call `doctest.testmod()`:

```python run id=first-use
"""Helpers for a bank account.

>>> balance_after(100, [30, 20])
50
"""


def balance_after(balance, withdrawals):
    """Return the balance after the withdrawals, in order.

    >>> balance_after(100, [])
    100
    >>> balance_after(100, [30, 70])
    0
    >>> balance_after(100, [150])
    Traceback (most recent call last):
        ...
    ValueError: balance 100, asked 150
    """
    for amount in withdrawals:
        if amount > balance:
            raise ValueError(f"balance {balance}, asked {amount}")
        balance -= amount
    return balance


if __name__ == "__main__":
    import doctest

    print(doctest.testmod())
```

```output for=first-use
TestResults(failed=0, attempted=4)
```

`testmod` checks the docstring of the module and the docstring of the function: 4 examples in all. When all examples pass, `testmod` prints nothing and returns the counts; this program prints the counts. When an example fails, `testmod` prints a report of it.

From the command line, `python -m doctest -v example.py` runs `testmod()` on a file, and the option `-v` prints each example that it tries.[^1]

## Concepts of the tool

**Which docstrings it reads.** "The module docstring, and all function, class and method docstrings are searched. Objects imported into the module are not searched."[^1] A module can also put tests in a variable `__test__`, so that they are not part of the help text.[^1]

**How it finds an example.** An example starts at a line with `>>>`, and the lines of a statement that goes on start with `...`. "Any expected output must immediately follow the final '>>> ' or '... ' line containing the code, and the expected output (if any) extends to the next '>>> ' or all-whitespace line."[^1] To expect a blank line in the output, write `<BLANKLINE>`.[^1]

**The execution context.** Each docstring runs in its own copy of the globals of the module. The documentation says that doctest "uses a shallow copy of M ’s globals, so that running tests doesn’t change the module’s real globals, and so that one test in M can’t leave behind crumbs that accidentally allow another test to work."[^1] The examples of one docstring can use names from earlier examples in the same docstring, but not names from other docstrings.[^1] Each docstring is therefore the fixture of its own examples.

**Exceptions.** An example can expect an exception. The expected output starts with the header `Traceback (most recent call last):`, and doctest ignores the stack lines after the header.[^1] It compares the last part: the type of the exception and its detail. The first example above uses `...` in place of the stack.

**Exact output.** doctest compares text, not values. The documentation warns: "doctest is serious about requiring exact matches in expected output. If even a single character doesn’t match, the test fails."[^1] The following program runs three examples from a text. The first example expects `0.3`, but Python prints the exact value of the float sum.

```python run id=exact-output
import doctest

text = """
>>> 0.1 + 0.2
0.3
>>> round(0.1 + 0.2, 2)
0.3
>>> list(range(12))  # doctest: +NORMALIZE_WHITESPACE
[0, 1, 2, 3, 4, 5,
 6, 7, 8, 9, 10, 11]
"""

test = doctest.DocTestParser().get_doctest(text, {}, "sums", "sums.txt", 0)
runner = doctest.DocTestRunner()
runner.run(test)
print(runner.summarize(verbose=False))
```

```output for=exact-output
**********************************************************************
File "sums.txt", line 2, in sums
Failed example:
    0.1 + 0.2
Expected:
    0.3
Got:
    0.30000000000000004
**********************************************************************
1 item had failures:
   1 of   3 in sums
***Test Failed*** 1 failure.
TestResults(failed=1, attempted=3)
```

The report gives the line, the example, the expected output and the output that the example gave. The second example passes, because it rounds the value before it prints it. The documentation gives other outputs that change from run to run: the order of the elements of a set, and the default `repr` of an object, which "embeds an address".[^1]

**Option flags and directives.** Option flags change how doctest compares the output. With `NORMALIZE_WHITESPACE`, "all sequences of whitespace (blanks and newlines) are treated as equal".[^1] The third example above uses it to split a long line. With `ELLIPSIS`, "an ellipsis marker ( ... ) in the expected output can match any substring in the actual output."[^1] A directive turns on a flag for one example, with a comment such as `# doctest: +ELLIPSIS`.[^1] The argument `optionflags` of `testmod` and `testfile` turns a flag on for all examples.[^1]

## Common tasks

```text
python -m doctest -v example.py
python -m doctest -v example.txt
python -m doctest -o ELLIPSIS -f example.py
```

The command `python -m doctest` runs `testmod()` on a `.py` file. For a file whose name does not end in `.py`, it runs `testfile()`.[^1] The option `-o` turns on an option flag, and `-f` stops at the first failure, as a short form of `-o FAIL_FAST`.[^1]

In code, `doctest.testfile("example.txt")` checks the examples in a text file, and it looks for the file in the directory of the calling module by default.[^1]

To run doctests with the other tests of a project:

- With `unittest`, add a function `load_tests` to a test module, and add the suite that `doctest.DocTestSuite(module)` returns. `doctest.DocFileSuite` does the same for text files.[^1]
- With pytest, the option `--doctest-modules` runs the examples in the docstrings of all modules.[^2] pytest also runs the files that match `test*.txt` through doctest by default, and `--doctest-glob` changes the pattern.[^2]

## Configuration

doctest has no configuration file. Set the option flags in three places:[^1]

- In a directive comment on one example, such as `# doctest: +NORMALIZE_WHITESPACE`.
- In the argument `optionflags` of `testmod`, `testfile`, `DocTestSuite` or `DocFileSuite`.
- With the option `-o` of `python -m doctest`.

The argument `globs` of `testmod` and `testfile` gives the examples a dictionary of names in place of the globals of the module.[^1]

## Alternatives

**unittest** and **pytest** are better for many small checks and for edge cases, because their tests are code and not text that must match exactly. The doctest documentation itself warns that "filling your docstrings with obscure test cases makes for bad documentation."[^1] A common choice is to keep a few clear examples in the docstrings, check them with doctest, and put the other tests in [[python-unittest]] or [[pytest]] tests. Both can run the doctests in the same run.

## History and versions

Tim Peters wrote doctest. The header of the source file says "Released to the public domain 16-Jan-2001, by Tim Peters", and it names Jim Fulton and Edward Loper for "Major enhancements and refactoring".[^3] doctest came into the standard library in Python 2.1, together with `unittest`.[^4] The notes for that release say: "The doctest module, contributed by Tim Peters, provides a testing framework based on running embedded examples in docstrings and comparing the results against the expected output."[^4] The final release of Python 2.1 was on April 17, 2001.[^4]

| Python | Change |
|---|---|
| 2.1 | The module `doctest` is added[^4] |
| 3.4 | The options `-o` and `-f` of the command line[^1] |
| 3.5 | `DocTestSuite` returns an empty suite for a module with no docstrings, in place of an error[^1] |
| 3.13 | Colour in the output by default[^1] |
| 3.15 | `DocTestSuite` runs each example as a subtest[^1] |

## References
