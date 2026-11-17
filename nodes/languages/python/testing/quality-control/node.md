---
tag: CW4SGC
kind: feature
owner: python
slug: quality-control
name: Quality Control
aliases: [doctest, doctest.testmod, tests in docstrings, unittest.TestCase, first test case, Python testing basics]
area: languages/python/testing
level: intro
summary: >-
  The standard library has two test tools. doctest runs the interactive examples in docstrings and checks their output, and unittest runs test methods of TestCase classes kept in separate files.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/stdlib.html#quality-control", locator: "The Python Tutorial, 10.11. Quality control"}
see_also: [python/unittest, python/testing-with-pytest, python/assert-statement, doctest-python, python-unittest, pytest]
sources:
  - url: https://docs.python.org/3.15/tutorial/stdlib.html#quality-control
    title: The Python Tutorial, 10. Brief tour of the standard library
    locator: 10.11. Quality control
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/doctest.html
    title: doctest, Python 3.15 documentation
    locator: module introduction; simple usage; what about exceptions?; option flags; testmod(); unittest API; warnings; soapbox
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/unittest.html
    title: unittest, Python 3.15 documentation
    locator: module introduction; basic example
    accessed: 2026-11-16
  - url: https://pkg.go.dev/testing
    title: Package testing, Go standard library
    locator: Examples
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
def average(values):
    """Computes the arithmetic mean of a list of numbers.

    >>> print(average([20, 30, 70]))
    40.0
    """
    return sum(values) / len(values)

import doctest
print(doctest.testmod())
```

```output for=synopsis
TestResults(failed=0, attempted=1)
```

The docstring holds an example copied from an interactive session. `doctest.testmod()` runs it, compares the output with the text below it, and returns the counts.

## Semantics

**r1.** The tutorial's advice: "One approach for developing high quality software is to write tests for each function as it is developed and to run those tests frequently during the development process."[^1]

**r2.** "The doctest module provides a tool for scanning a module and validating tests embedded in a program’s docstrings. Test construction is as simple as cutting-and-pasting a typical call along with its results into the docstring."[^1] doctest "searches for pieces of text that look like interactive Python sessions, and then executes those sessions to verify that they work exactly as shown."[^2]

**r3.** `testmod()` tests the examples in docstrings "reachable from module m (or module __main__ if m is not supplied or is None)", and it returns "(failure_count, test_count)."[^2] A run with no failures prints nothing: "This won’t display anything unless an example fails".[^2] With `verbose=True` or the `-v` switch, "a detailed report of all examples tried is printed to standard output".[^2] Example: [`failure`](#failure).

**r4.** An example can expect an exception: "just paste in the traceback."[^2] The stack lines between the header and the last line "are ignored by doctest."[^2] Example: [`exception`](#exception).

**r5.** "The unittest module is not as effortless as the doctest module, but it allows a more comprehensive set of tests to be maintained in a separate file".[^1] Tests are methods of a class: "In unittest, test cases are represented by unittest.TestCase instances."[^3] Example: [`unittest`](#unittest).

**r6.** The unittest framework "was originally inspired by JUnit and has a similar flavor as major unit testing frameworks in other languages."[^3] doctest can feed it: `doctest.DocTestSuite()` will "Convert doctest tests for a module to a unittest.TestSuite."[^2]

## Examples

```python run id=failure
def average(values):
    """Computes the arithmetic mean of a list of numbers.

    >>> print(average([20, 30, 70]))
    41.0
    """
    return sum(values) / len(values)

import doctest
print(doctest.testmod())
```

```output for=failure
**********************************************************************
File "$WORK/main.py", line 4, in __main__.average
Failed example:
    print(average([20, 30, 70]))
Expected:
    41.0
Got:
    40.0
**********************************************************************
1 items had failures:
   1 of   1 in __main__.average
***Test Failed*** 1 failures.
TestResults(failed=1, attempted=1)
```

The docstring is wrong on purpose. doctest prints the example, the expected text and the actual output. "the final line of output is ***Test Failed*** N failures., where N is the number of examples that failed."[^2]

```python run id=exception
def parse_port(text):
    """Return the port number in text.

    >>> parse_port('8080')
    8080
    >>> parse_port('http')
    Traceback (most recent call last):
      ...
    ValueError: invalid literal for int() with base 10: 'http'
    """
    return int(text)

import doctest
print(doctest.testmod(verbose=True))
```

```output for=exception
Trying:
    parse_port('8080')
Expecting:
    8080
ok
Trying:
    parse_port('http')
Expecting:
    Traceback (most recent call last):
      ...
    ValueError: invalid literal for int() with base 10: 'http'
ok
1 items had no tests:
    __main__
1 items passed all tests:
   2 tests in __main__.parse_port
2 tests in 2 items.
2 passed and 0 failed.
Test passed.
TestResults(failed=0, attempted=2)
```

The verbose report shows each example. The `...` line stands for the traceback stack, which doctest skips (rule r4). The module docstring has no examples, so `__main__` counts as an item with no tests.

```python test id=unittest
import unittest


def average(values):
    return sum(values) / len(values)


class TestStatisticalFunctions(unittest.TestCase):
    def test_average(self):
        self.assertEqual(average([20, 30, 70]), 40.0)
        self.assertEqual(round(average([1, 5, 7]), 1), 4.3)
        with self.assertRaises(ZeroDivisionError):
            average([])
        with self.assertRaises(TypeError):
            average(20, 30, 70)


if __name__ == "__main__":
    unittest.main()
```

```output stream=stderr
test_average (test_main.TestStatisticalFunctions.test_average) ... ok

----------------------------------------------------------------------
Ran 1 test in $TIME

OK
```

This is the tutorial's test, run with `python -m unittest -v`. Each `assert` method checks one fact, and `assertRaises` checks that the block raises the exception. In the tutorial, `unittest.main()` at the end runs the tests: "Calling from the command line invokes all tests".[^1]

## Pitfalls

doctest compares text, character by character. "doctest is serious about requiring exact matches in expected output. If even a single character doesn’t match, the test fails."[^2] A set prints in no fixed order, and the default `repr()` of an object holds its address. The docs suggest sorting, or comparing in the example:

```python run id=exact
def tags():
    """
    >>> sorted(tags())
    ['io', 'os']
    >>> tags() == {'os', 'io'}
    True
    >>> object()  # doctest: +ELLIPSIS
    <object object at 0x...>
    """
    return {'os', 'io'}

import doctest
print(doctest.testmod())
```

```output for=exact
TestResults(failed=0, attempted=3)
```

With the ELLIPSIS flag, "an ellipsis marker (...) in the expected output can match any substring in the actual output."[^2]

Keep docstring examples for the reader. "filling your docstrings with obscure test cases makes for bad documentation."[^2] Put the edge cases in a unittest or pytest file.

`testmod()` only looks at docstrings of the module it tests: "Only docstrings attached to objects belonging to module m are searched."[^2]

## Comparison

Go has the same idea as doctest in its example functions. They "may include a concluding line comment that begins with "Output:" and is compared with the standard output of the function when the tests are run."[^4] Go compares the printed output of a function, while doctest compares the echo of each line of an interactive session. Go's other tests are `TestXxx` functions in a file with "a name ending in "_test.go"."[^4] They play the part of unittest test methods.
