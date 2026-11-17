---
tag: K8037R
kind: feature
owner: python
slug: unittest-mock
name: unittest.mock
aliases: [unittest.mock, Mock, MagicMock, patch, mock.patch, patch.object, create_autospec, autospec, side_effect, return_value, assert_called_once_with, where to patch]
area: languages/python/testing
level: core
summary: >-
  unittest.mock replaces parts of a program with Mock objects that record their calls. patch swaps a name for a mock during one test, and autospec makes the mock keep the real signature.
grammar: "unittest.mock.patch(target, new=DEFAULT, spec=None, create=False, spec_set=None, autospec=None, new_callable=None, **kwargs)"
spec: {standard: null, url: "https://docs.python.org/3.15/library/unittest.mock.html", locator: "unittest.mock, mock object library"}
see_also: [python/unittest, python/testing-with-pytest, python/quality-control, test-double, python-unittest, pytest]
sources:
  - url: https://docs.python.org/3.15/library/unittest.mock.html
    title: unittest.mock, Python 3.15 documentation
    locator: module introduction; quick guide; the Mock class; patch(); where to patch; autospeccing
    accessed: 2026-11-16
  - url: https://pkg.go.dev/testing/iotest
    title: Package iotest, Go standard library
    locator: package overview; ErrReader
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
from unittest.mock import Mock


def report(fetch, city):
    data = fetch(city)
    return f"{city}: {data['temp']} C"


fetch = Mock(return_value={'temp': 21})
print(report(fetch, 'Oslo'))
fetch.assert_called_once_with('Oslo')
print(fetch.call_count, fetch.call_args)
```

```output for=synopsis
Oslo: 21 C
1 call('Oslo')
```

`report` needs something to call. The test gives it a `Mock` that returns a fixed dict, then asks the mock how it was called: once, with `'Oslo'`.

## Semantics

**r1.** "unittest.mock is a library for testing in Python. It allows you to replace parts of your system under test with mock objects and make assertions about how they have been used."[^1] It "is based on the ‘action -> assertion’ pattern instead of ‘record -> replay’ used by many mocking frameworks."[^1] The test runs the code first and checks the calls after.

**r2.** "Mocks are callable and create attributes as new mocks when you access them". "Accessing the same attribute will always return the same mock. Mocks record how you use them, allowing you to make assertions about what your code has done to them."[^1] The record is in `called`, `call_count`, `call_args` and `call_args_list`, and methods such as `assert_called_once_with` check it.

**r3.** `return_value` is "The value returned when the mock is called."[^1] `side_effect` overrides it. It can be a function: "The function is called with the same arguments as the mock, and unless it returns DEFAULT, the return value of this function is used as the return value."[^1] It can be an exception, which "will be raised when the mock is called", or an iterable, and then "each call to the mock will return the next value from the iterable."[^1]

**r4.** "MagicMock is a subclass of Mock with all the magic methods pre-created and ready to use."[^1] A plain `Mock` does not support `len()`, iteration or `with` unless those methods are set by hand.

**r5.** `patch()` "acts as a function decorator, class decorator or a context manager. Inside the body of the function or with statement, the target is patched with a new object. When the function/with statement exits the patch is undone."[^1] "If patch() is used as a decorator and new is omitted, the created mock is passed in as an extra argument to the decorated function. If patch() is used as a context manager the created mock is returned by the context manager."[^1] The target is a string such as `'package.module.ClassName'`, and "The target is imported when the decorated function is executed, not at decoration time."[^1] `patch.object(obj, 'name')` patches an attribute of an object that the test already holds.

**r6.** Where to patch: "patch() works by (temporarily) changing the object that a name points to with another one. There can be many names pointing to any individual object, so for patching to work you must ensure that you patch the name used by the system under test."[^1] "The basic principle is that you patch where an object is looked up, which is not necessarily the same place as where it is defined."[^1] After `from a import SomeClass` in module `b`, the patch target is `'b.SomeClass'`; after `import a`, it is `'a.SomeClass'`.[^1]

**r7.** A mock accepts any call and any attribute, so a test can pass while the code calls the real object wrongly: "If you refactor some of your code, rename members and so on, any tests for code that is still using the old api but uses mocks instead of the real objects will still pass."[^1] `spec`, `spec_set` and autospec close the gap. "Auto-speccing creates mock objects that have the same attributes and methods as the objects they are replacing, and any functions and methods (including constructors) have the same call signature as the real object."[^1] It is done "through the autospec argument to patch, or the create_autospec() function."[^1]

**r8.** Misspelled assertions are caught: "By default, accessing any attribute whose name starts with assert, assret, asert, aseert or assrt will raise an AttributeError."[^1]

## Examples

### side_effect: errors and computed values

```python run id=side-effect
from unittest.mock import Mock

flaky = Mock(side_effect=[ConnectionError('down'), 'ok'])
try:
    flaky()
except ConnectionError as e:
    print('first call:', e)
print('second call:', flaky())

double = Mock(side_effect=lambda x: x * 2)
print(double(21), double.call_args_list)
```

```output for=side-effect
first call: down
second call: ok
42 [call(21)]
```

A list of side effects scripts a sequence of calls. Here the first call fails and the second works, which is how a test checks retry code.

### MagicMock and protocols

```python run id=magicmock
from unittest.mock import MagicMock, Mock

box = MagicMock()
box.__len__.return_value = 3
box.__iter__.return_value = iter(['a', 'b', 'c'])
print(len(box), list(box))

with box as inside:
    print(inside is box.__enter__.return_value)

try:
    len(Mock())
except TypeError as e:
    print('Mock:', type(e).__name__)
```

```output for=magicmock
3 ['a', 'b', 'c']
True
Mock: TypeError
```

### Patch where the name is looked up

```python norun reason=fragment group=dice file=dice.py
from random import randint


def roll():
    return randint(1, 6)
```

```python run group=dice file=main.py id=where
from unittest.mock import patch

import dice

with patch('random.randint', return_value=6) as fake:
    dice.roll()
    print('random.randint patched, mock called:', fake.called)

with patch('dice.randint', return_value=6) as fake:
    print('dice.randint patched:', dice.roll(), fake.called)
```

```output for=where
random.randint patched, mock called: False
dice.randint patched: 6 True
```

`dice` copied the name `randint` into its own namespace when it was imported. Patching `random.randint` changes the `random` module, but `roll` looks up `dice.randint`, so the first mock is never called.

### patch in a unittest test

```python test id=patch-test
import os
import unittest
from unittest import mock


def config_path():
    return os.path.join(os.path.expanduser('~'), '.apprc')


class ConfigPathTest(unittest.TestCase):
    @mock.patch('os.path.expanduser', return_value='/home/ada')
    def test_decorator(self, expanduser):
        self.assertEqual(config_path(), '/home/ada/.apprc')
        expanduser.assert_called_once_with('~')

    def test_context_manager(self):
        with mock.patch('os.path.expanduser', return_value='/root') as fake:
            self.assertEqual(config_path(), '/root/.apprc')
        self.assertEqual(fake.call_count, 1)
        self.assertNotEqual(os.path.expanduser, fake)
```

```output for=patch-test stream=stderr
test_context_manager (test_main.ConfigPathTest.test_context_manager) ... ok
test_decorator (test_main.ConfigPathTest.test_decorator) ... ok

----------------------------------------------------------------------
Ran 2 tests in $TIME

OK
```

The decorator passes the mock in as the extra argument `expanduser`. The context manager form returns it from `with`, and after the block the real `os.path.expanduser` is back.

### Autospec checks the signature

```python run id=autospec
from unittest.mock import Mock, create_autospec


def send(to, subject, body):
    pass


loose = Mock()
loose(1, 2, 3, 4)
print('Mock accepted 4 arguments')

strict = create_autospec(send, return_value=True)
print(strict('ada@example.com', 'hi', 'text'))
try:
    strict('ada@example.com')
except TypeError as e:
    print('autospec:', e)
```

```output for=autospec
Mock accepted 4 arguments
True
autospec: missing a required argument: 'subject'
```

### A misspelled assertion

```python fails id=typo expect=exception:AttributeError
from unittest.mock import Mock

m = Mock()
m.assret_called_once()
```

`assret_called_once` is not a method of `Mock`. Without the check in r8, the mock would create it as a new attribute, the call would return another mock, and the test would pass without checking anything.

## Pitfalls

- Patching the module where a function is defined, when the code under test did `from module import name`, has no effect (r6). The example above shows the mock never being called. Patch the name in the module that uses it.
- A plain `Mock` accepts any arguments and any attribute name (r7). After a rename or a signature change in the real code, tests that use loose mocks still pass. Use `autospec=True` with `patch`, or `create_autospec`.
- A misspelled assertion method on a mock with a `spec`, or one whose name does not start with the prefixes in r8, such as `mock.called_once_with(...)`, is just another attribute access and checks nothing. Call assertion methods by their exact names.
- `return_value` is ignored while `side_effect` is a function that returns something other than `DEFAULT` (r3). Set only one of them.
- A test that mocks everything a unit touches does not check how the units fit together. The docs say: "Note that this is another reason why you need integration tests as well as unit tests."[^1]

## Comparison

Go's standard library has helpers for writing fakes. `testing/iotest` "implements Readers and Writers useful mainly for testing."[^2] For example, "ErrReader returns an io.Reader that returns 0, err from all Read calls."[^2] A function that takes an `io.Reader` can be given `iotest.ErrReader(...)` to test its error path, the job that `side_effect` with an exception does for a Python mock.

[^1]: unittest.mock, Python 3.15 documentation.
[^2]: Package iotest, Go standard library.
