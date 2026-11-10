```python test id=cart
import unittest


def cart_total(items):
    """Return the total in cents of a list of (price, quantity) pairs."""
    total = 0
    for price, quantity in items:
        if quantity < 0:
            raise ValueError(f"negative quantity: {quantity}")
        total += price * quantity
    return total


class CartTotalTest(unittest.TestCase):
    def test_empty_cart_is_zero(self):
        self.assertEqual(cart_total([]), 0)

    def test_sums_price_times_quantity(self):
        self.assertEqual(cart_total([(250, 2), (100, 1)]), 600)

    def test_rejects_negative_quantity(self):
        with self.assertRaises(ValueError):
            cart_total([(250, -1)])


if __name__ == "__main__":
    unittest.main()
```

```output stream=stderr
test_empty_cart_is_zero (test_main.CartTotalTest.test_empty_cart_is_zero) ... ok
test_rejects_negative_quantity (test_main.CartTotalTest.test_rejects_negative_quantity) ... ok
test_sums_price_times_quantity (test_main.CartTotalTest.test_sums_price_times_quantity) ... ok

----------------------------------------------------------------------
Ran 3 tests in $TIME

OK
```

- Verdict: stdlib. The `unittest` module is part of the standard library. A test case is a subclass of `unittest.TestCase`, and each method whose name starts with `test` is one test.
- `unittest` writes its report to standard error. Each dot is one test that passed. A failure prints `F` and an error prints `E`, followed by a traceback.
- `assertRaises` used as a context manager checks that the block raises the exception. The test fails when the block returns normally.
- Many projects use pytest, which is a library. It collects plain functions whose names start with `test_` and rewrites the `assert` statement so that a failure shows the values on each side.
