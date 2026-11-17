---
tag: N3D6DD
kind: feature
owner: python
slug: testing-async-code
name: Testing async code
aliases: [IsolatedAsyncioTestCase, asyncSetUp, asyncTearDown, addAsyncCleanup, pytest-asyncio, pytest.mark.asyncio, asyncio_mode, anyio pytest plugin, pytest.mark.anyio, anyio_backend, testing coroutines]
area: languages/python/testing
level: advanced
summary: >-
  A coroutine test needs an event loop to run it. unittest has IsolatedAsyncioTestCase; pytest needs a plugin such as pytest-asyncio or AnyIO's, and fails async tests without one.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/unittest.html#unittest.IsolatedAsyncioTestCase", locator: "unittest.IsolatedAsyncioTestCase"}
see_also: [python/unittest, python/testing-with-pytest, python/asyncio, python/coroutines-async-and-await, pytest]
sources:
  - url: https://docs.python.org/3.15/library/asyncio-dev.html
    title: Developing with asyncio, Python 3.15 documentation
    locator: Debug Mode; Detect never-awaited coroutines
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/unittest.html
    title: unittest, Python 3.15 documentation
    locator: IsolatedAsyncioTestCase
    accessed: 2026-11-16
  - url: https://pytest-asyncio.readthedocs.io/en/stable/concepts.html
    title: Concepts, pytest-asyncio documentation
    locator: asyncio event loops; Test discovery modes; Test execution and concurrency
    accessed: 2026-11-16
  - url: https://anyio.readthedocs.io/en/stable/testing.html
    title: Testing with AnyIO, AnyIO documentation
    locator: Creating asynchronous tests; Specifying the backends to run on
    accessed: 2026-11-16
  - url: https://pkg.go.dev/testing
    title: Package testing, Go standard library
    locator: package overview; T.Parallel
    accessed: 2026-11-16
---

## Synopsis

```python test id=synopsis
import asyncio
import unittest


async def fetch(key, delay=0.01):
    await asyncio.sleep(delay)
    return key.upper()


class FetchTest(unittest.IsolatedAsyncioTestCase):
    async def asyncSetUp(self):
        # Debug mode logs any step slower than 0.1 s; raise the limit so
        # a busy machine adds no log lines to the test output.
        asyncio.get_running_loop().slow_callback_duration = 10

    async def test_debug_mode(self):
        self.assertTrue(asyncio.get_running_loop().get_debug())

    async def test_fetch(self):
        self.assertEqual(await fetch('a'), 'A')

    async def test_gather(self):
        results = await asyncio.gather(fetch('x'), fetch('y'))
        self.assertEqual(results, ['X', 'Y'])

    async def test_timeout(self):
        with self.assertRaises(TimeoutError):
            await asyncio.wait_for(fetch('slow', delay=5), timeout=0.05)
```

```output for=synopsis stream=stderr
test_debug_mode (test_main.FetchTest.test_debug_mode) ... ok
test_fetch (test_main.FetchTest.test_fetch) ... ok
test_gather (test_main.FetchTest.test_gather) ... ok
test_timeout (test_main.FetchTest.test_timeout) ... ok

----------------------------------------------------------------------
Ran 4 tests in $TIME

OK
```

The test methods are coroutines. `IsolatedAsyncioTestCase` runs each one in an event loop, so the body can `await` the code under test, `gather` several calls, and check a timeout with `assertRaises`. `test_debug_mode` shows that the loop runs in asyncio's debug mode, where "Callbacks taking longer than 100 milliseconds are logged."[^1] The log line would land in the test output, so `asyncSetUp` raises the limit through `loop.slow_callback_duration`, the attribute that sets "the minimum execution duration in seconds that is considered" slow.[^1]

## Semantics

**r1.** `unittest.IsolatedAsyncioTestCase` "provides an API similar to TestCase and also accepts coroutines as test functions."[^2] Its `run()` "Sets up a new event loop to run the test", and "At the end of the test all the tasks in the event loop are cancelled."[^2]

**r2.** The class adds async fixtures. `asyncSetUp()` "is called after TestCase.setUp()", and `asyncTearDown()` "is called before tearDown()".[^2] `addAsyncCleanup()` "accepts a coroutine that can be used as a cleanup function."[^2] The docs give the order as setUp, asyncSetUp, the test, asyncTearDown, tearDown and the cleanups.[^2]

**r3.** pytest itself runs only plain functions. An `async def` test with no async plugin installed fails with the message "async def functions are not natively supported", followed by a list of plugins to install (see the example below).

**r4.** pytest-asyncio "provides one asyncio event loop for each pytest collector. By default, each test runs in the event loop provided by the Function collector, i.e. tests use the loop with the narrowest scope."[^3] The `loop_scope` argument of the `asyncio` mark lets tests in one class, module or session share a loop.[^3]

**r5.** pytest-asyncio has two modes. "In strict mode pytest-asyncio will only run tests that have the asyncio marker and will only evaluate async fixtures decorated with @pytest_asyncio.fixture."[^3] Strict is the default because "Pytest automatically enables installed plugins. As a result pytest plugins need to coexist peacefully in their default configuration."[^3] "In auto mode pytest-asyncio automatically adds the asyncio marker to all asynchronous test functions."[^3] Tests still run one after another: "pytest-asyncio runs async tests sequentially, just like how pytest runs synchronous tests."[^3]

**r6.** AnyIO ships its own pytest plugin, which "is part of the AnyIO distribution, so nothing extra needs to be installed to use it."[^4] Tests are picked up through the `anyio_mode` option set to auto, the `pytest.mark.anyio` marker or the `anyio_backend` fixture.[^4] "The anyio_backend fixture determines the backends and their options that tests and fixtures are run with."[^4] Overriding it in `conftest.py` to return `'asyncio'` runs the tests on asyncio only; by default they run on every supported backend.[^4]

**r7.** Calling a coroutine function without `await` does not run its body. "When a coroutine function is called, but not awaited" "or the coroutine is not scheduled with asyncio.create_task(), asyncio will emit a RuntimeWarning".[^1] A warning does not fail a test.

## Examples

### The order of the fixture methods

```python run id=order
import io
import unittest

events = []


class Test(unittest.IsolatedAsyncioTestCase):
    def setUp(self):
        events.append('setUp')

    async def asyncSetUp(self):
        events.append('asyncSetUp')
        self.addAsyncCleanup(self.on_cleanup)

    async def test_response(self):
        events.append('test_response')

    async def asyncTearDown(self):
        events.append('asyncTearDown')

    def tearDown(self):
        events.append('tearDown')

    async def on_cleanup(self):
        events.append('cleanup')


suite = unittest.TestLoader().loadTestsFromTestCase(Test)
unittest.TextTestRunner(stream=io.StringIO()).run(suite)
print(events)
```

```output for=order
['setUp', 'asyncSetUp', 'test_response', 'asyncTearDown', 'tearDown', 'cleanup']
```

This is the class from the unittest docs, run with a programmatic runner. The sync and async fixture methods nest around the test, and the cleanup added in `asyncSetUp` runs last.

### pytest without an async plugin

```python norun reason=fragment group=plain file=test_async.py
import asyncio


async def fetch(key):
    await asyncio.sleep(0.01)
    return key.upper()


async def test_fetch():
    assert await fetch('a') == 'B'


def test_fetch_with_run():
    assert asyncio.run(fetch('a')) == 'A'
```

```python run group=plain file=main.py id=no-plugin
import pytest

code = pytest.main(["-qq", "-rA", "-p", "no:cacheprovider",
                    "-p", "no:asyncio", "-p", "no:anyio", "test_async.py"])
print("exit code:", int(code))
```

```output for=no-plugin
F.                                                                       [100%]
=================================== FAILURES ===================================
__________________________________ test_fetch __________________________________
async def functions are not natively supported.
You need to install a suitable plugin for your async framework, for example:
  - anyio
  - pytest-asyncio
  - pytest-tornasync
  - pytest-trio
  - pytest-twisted
==================================== PASSES ====================================
=========================== short test summary info ============================
PASSED test_async.py::test_fetch_with_run
FAILED test_async.py::test_fetch - Failed: async def functions are not native...
exit code: 1
```

The machine that ran these examples has pytest-asyncio and AnyIO installed, so the two `-p no:` options switch their plugins off. `test_fetch` fails before its body runs; its wrong expectation `'B'` is never checked. `test_fetch_with_run` passes because it is a plain function that runs its own loop with `asyncio.run`, which works for a single call but gives no async fixtures.

### With pytest-asyncio

The same test with pytest-asyncio installed, in its default strict mode:

```python norun reason=fragment group=pa file=test_pa.py
import asyncio

import pytest
import pytest_asyncio


async def fetch(key):
    await asyncio.sleep(0.01)
    return key.upper()


@pytest_asyncio.fixture
async def keys():
    yield ['a', 'b']


@pytest.mark.asyncio
async def test_fetch(keys):
    assert [await fetch(k) for k in keys] == ['A', 'B']
```

```python run group=pa file=main.py id=pytest-asyncio
import pytest

code = pytest.main(["-qq", "-rA", "-p", "no:cacheprovider",
                    "-o", "asyncio_default_fixture_loop_scope=function", "test_pa.py"])
print("exit code:", int(code))
```

```output for=pytest-asyncio
.                                                                        [100%]
==================================== PASSES ====================================
=========================== short test summary info ============================
PASSED test_pa.py::test_fetch
exit code: 0
```

The output is from pytest 9.2.0 with pytest-asyncio 1.5.0. That version warns when `asyncio_default_fixture_loop_scope` is unset, so the run sets it with `-o`; a project sets it once in its pytest configuration.

With `asyncio_mode = "auto"` in the pytest configuration, the mark and the special fixture decorator can be dropped (r5).

### With AnyIO

```python norun reason=fragment group=an file=test_an.py
import anyio
import pytest


@pytest.fixture
def anyio_backend():
    return 'asyncio'


@pytest.mark.anyio
async def test_sleep():
    await anyio.sleep(0)
```

```python run group=an file=main.py id=anyio
import pytest

code = pytest.main(["-qq", "-rA", "-p", "no:cacheprovider", "-p", "no:asyncio", "test_an.py"])
print("exit code:", int(code))
```

```output for=anyio
.                                                                        [100%]
==================================== PASSES ====================================
=========================== short test summary info ============================
PASSED test_an.py::test_sleep
exit code: 0
```

The output is from AnyIO 4.16.0. `-p no:asyncio` leaves only the AnyIO plugin in play, which keeps the two plugins from clashing (see Pitfalls).

The `anyio_backend` fixture is usually put in `conftest.py`. Without it, the test runs once per backend (r6).

### An async test on the wrong base class

```python run id=wrong-base
import io
import unittest
import warnings


class WrongBaseTest(unittest.TestCase):
    async def test_never_runs(self):
        self.assertEqual(1, 2)


suite = unittest.TestLoader().loadTestsFromTestCase(WrongBaseTest)
with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter('always')
    result = unittest.TextTestRunner(stream=io.StringIO()).run(suite)
print('successful:', result.wasSuccessful())
for w in caught:
    print(w.category.__name__)
```

```output for=wrong-base
successful: True
RuntimeWarning
DeprecationWarning
```

`assertEqual(1, 2)` should fail, but the class is a plain `TestCase`. The runner calls the method, gets a coroutine object back, and never runs it. The run is green, with only a RuntimeWarning for the coroutine that was never awaited (r7) and a DeprecationWarning for a test method that returned a value.

## Pitfalls

- An `async def` test in a plain `unittest.TestCase` passes without running (see the last example). Use `IsolatedAsyncioTestCase`, and run tests with warnings shown so the RuntimeWarning is seen.
- In pytest, an `async def` test needs a plugin (r3). With pytest-asyncio in strict mode, an async test without `@pytest.mark.asyncio`, or an async fixture made with `@pytest.fixture` instead of `@pytest_asyncio.fixture`, is not handled by the plugin (r5).
- pytest-asyncio's auto mode and AnyIO's auto mode conflict. The AnyIO docs say its auto mode "does not work if pytest-asyncio is installed and configured to use its own auto mode".[^4]
- A test that starts a task and does not await it may end before the task runs. `IsolatedAsyncioTestCase` cancels the tasks still pending at the end of the test (r1), so a check inside that task never happens. Await the task, or use `asyncio.gather` or a `TaskGroup`.
- Real sleeps make a suite slow. Keep delays in tests small, as in the synopsis, and use `asyncio.wait_for` with a short timeout so that a hung coroutine fails the test instead of hanging the run.

## Comparison

Go has no async test runner to choose. `go test` "automates execution of any function of the form func TestXxx(*testing.T)", and the same kind of function tests code that starts goroutines.[^5] To run tests at the same time, a test calls `t.Parallel`, which "signals that this test is to be run in parallel with (and only with) other parallel tests."[^5] Python's async test runners do the opposite by default: pytest-asyncio "runs async tests sequentially".[^3]

[^2]: unittest, Python 3.15 documentation.
[^3]: Concepts, pytest-asyncio documentation.
[^4]: Testing with AnyIO, AnyIO documentation.
[^1]: Developing with asyncio, Python 3.15 documentation.
[^5]: Package testing, Go standard library.
