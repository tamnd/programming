```rust test id=account
#[derive(Debug, PartialEq)]
pub struct InsufficientFunds {
    pub balance: i64,
    pub asked: i64,
}

pub struct Account {
    balance: i64,
}

impl Account {
    pub fn new(balance: i64) -> Self {
        Account { balance }
    }

    pub fn balance(&self) -> i64 {
        self.balance
    }

    pub fn deposit(&mut self, amount: i64) {
        self.balance += amount;
    }

    pub fn withdraw(&mut self, amount: i64) -> Result<(), InsufficientFunds> {
        if amount > self.balance {
            return Err(InsufficientFunds { balance: self.balance, asked: amount });
        }
        self.balance -= amount;
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    // The fixture: each test calls this function to get its own account.
    fn account() -> Account {
        Account::new(100)
    }

    #[test]
    fn deposit_adds_to_balance() {
        let mut a = account();
        a.deposit(50);
        assert_eq!(a.balance(), 150);
    }

    #[test]
    fn withdraw_leaves_the_rest() {
        for (amount, rest) in [(30, 70), (100, 0)] {
            let mut a = account();
            a.withdraw(amount).unwrap();
            assert_eq!(a.balance(), rest, "after a withdrawal of {amount}");
        }
    }

    #[test]
    fn withdraw_more_than_balance_fails() {
        let mut a = account();
        assert_eq!(a.withdraw(150), Err(InsufficientFunds { balance: 100, asked: 150 }));
        assert_eq!(a.balance(), 100);
    }
}
```

```output

running 3 tests
test tests::deposit_adds_to_balance ... ok
test tests::withdraw_leaves_the_rest ... ok
test tests::withdraw_more_than_balance_fails ... ok

test result: ok. 3 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in $TIME
```

- Verdict: native. The `#[test]` attribute is part of the language. The compiler, with `--test`, builds a binary that runs each function that has the attribute. `cargo test` does this for each crate.
- Rust has no fixture hook. A test calls a plain function, such as `account`, to get a new value, and the value is dropped at the end of the test.
- Rust has no built-in parameterized test. A loop over a table checks each row in one test, and the message of `assert_eq!` names the row. The first row that fails stops the test. Crates such as rstest add parameterized tests and fixtures.
- The harness runs the tests on several threads by default. A test fails when it panics, and the attribute `#[should_panic]` marks a test that must panic.
