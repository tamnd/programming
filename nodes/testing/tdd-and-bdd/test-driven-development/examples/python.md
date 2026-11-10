```python test id=leap
import unittest


def is_leap(year):
    return year % 4 == 0 and (year % 100 != 0 or year % 400 == 0)


class LeapYearTest(unittest.TestCase):
    # The tests in the order of the to-do list. Each one added one clause.
    def test_1_year_divisible_by_4_is_leap(self):
        self.assertTrue(is_leap(2024))

    def test_2_other_year_is_common(self):
        self.assertFalse(is_leap(2023))

    def test_3_century_is_common(self):
        self.assertFalse(is_leap(1900))

    def test_4_century_divisible_by_400_is_leap(self):
        self.assertTrue(is_leap(2000))


if __name__ == "__main__":
    unittest.main()
```

```output stream=stderr
test_1_year_divisible_by_4_is_leap (test_main.LeapYearTest.test_1_year_divisible_by_4_is_leap) ... ok
test_2_other_year_is_common (test_main.LeapYearTest.test_2_other_year_is_common) ... ok
test_3_century_is_common (test_main.LeapYearTest.test_3_century_is_common) ... ok
test_4_century_divisible_by_400_is_leap (test_main.LeapYearTest.test_4_century_divisible_by_400_is_leap) ... ok

----------------------------------------------------------------------
Ran 4 tests in $TIME

OK
```

- Verdict: stdlib. `unittest` is in the standard library, and `python -m unittest` finds and runs the test files. The page on unit testing covers the runner.
- The runner sorts test methods by name. The numbers in the names keep the report in the order in which the cycle wrote the tests.
- The red step in Python happens at run time. A call to a function that does not exist raises `NameError` when the test runs, and the runner reports it as an error, not as a failure.

The second cycle, with the fake still in place, is a test that fails on purpose. The runner reports the failure and exits with a nonzero status.

```python test fails expect=test-failure id=red
import unittest


def is_leap(year):
    return True  # Fake It from the first cycle


class LeapYearTest(unittest.TestCase):
    def test_1_year_divisible_by_4_is_leap(self):
        self.assertTrue(is_leap(2024))

    def test_2_other_year_is_common(self):
        self.assertFalse(is_leap(2023))


if __name__ == "__main__":
    unittest.main()
```

```output stream=stderr
test_1_year_divisible_by_4_is_leap (test_main.LeapYearTest.test_1_year_divisible_by_4_is_leap) ... ok
test_2_other_year_is_common (test_main.LeapYearTest.test_2_other_year_is_common) ... FAIL

======================================================================
FAIL: test_2_other_year_is_common (test_main.LeapYearTest.test_2_other_year_is_common)
----------------------------------------------------------------------
Traceback (most recent call last):
  File "$WORK/test_main.py", line 13, in test_2_other_year_is_common
    self.assertFalse(is_leap(2023))
    ~~~~~~~~~~~~~~~~^^^^^^^^^^^^^^^
AssertionError: True is not false

----------------------------------------------------------------------
Ran 2 tests in $TIME

FAILED (failures=1)
```

- `unittest` tells a failure from an error. A failed assertion is a failure. Any other exception, such as `NameError`, is an error. Both are red, but a failure is the red that the cycle wants: the test ran and the check was false.
