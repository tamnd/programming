```python test id=account
import unittest


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


class AccountTest(unittest.TestCase):
    def setUp(self):
        # The fixture: unittest calls setUp before each test method.
        self.account = Account(100)

    def test_deposit_adds_to_balance(self):
        self.account.deposit(50)
        self.assertEqual(self.account.balance, 150)

    def test_withdraw_leaves_the_rest(self):
        for amount, rest in [(30, 70), (100, 0)]:
            with self.subTest(amount=amount):
                account = Account(100)
                account.withdraw(amount)
                self.assertEqual(account.balance, rest)

    def test_withdraw_more_than_balance_fails(self):
        with self.assertRaises(InsufficientFunds):
            self.account.withdraw(150)
        self.assertEqual(self.account.balance, 100)


if __name__ == "__main__":
    unittest.main()
```

```output stream=stderr
test_deposit_adds_to_balance (test_main.AccountTest.test_deposit_adds_to_balance) ... ok
test_withdraw_leaves_the_rest (test_main.AccountTest.test_withdraw_leaves_the_rest) ... ok
test_withdraw_more_than_balance_fails (test_main.AccountTest.test_withdraw_more_than_balance_fails) ... ok

----------------------------------------------------------------------
Ran 3 tests in $TIME

OK
```

- Verdict: stdlib. `unittest` is in the standard library. It finds the methods whose names start with `test` in each subclass of `unittest.TestCase`, and `unittest.main()` runs them and exits with the status.
- `setUp` is the fixture. `unittest` makes a new instance of the class for each test method and calls `setUp` first, so `test_withdraw_more_than_balance_fails` gets an account with 100, whatever the other tests did.
- `subTest` reports each row of a table as its own case. When one row fails, the other rows still run, and the report shows the parameters of the failed row. `setUp` runs once for the method, not once for each row, so the loop makes its own account.
- pytest also runs this file. It collects `unittest.TestCase` classes, and adds plain test functions and fixtures that are function arguments.
