```rust run id=private-by-default
mod bank {
    pub struct Account {
        balance: i64, // private: only code in mod bank can name it
        history: Vec<i64>,
    }

    impl Account {
        pub fn new() -> Self {
            Account { balance: 0, history: Vec::new() }
        }

        pub fn deposit(&mut self, amount: i64) -> Result<(), &'static str> {
            if amount <= 0 {
                return Err("deposit must be positive");
            }
            self.history.push(amount);
            self.balance += amount;
            Ok(())
        }

        pub fn withdraw(&mut self, amount: i64) -> Result<(), &'static str> {
            if amount > self.balance {
                return Err("insufficient funds");
            }
            self.history.push(-amount);
            self.balance -= amount;
            Ok(())
        }

        pub fn balance(&self) -> i64 {
            self.balance
        }

        pub fn history(&self) -> &[i64] {
            &self.history // a shared borrow: the caller can read, not change
        }
    }
}

fn main() {
    let mut a = bank::Account::new();
    a.deposit(100).unwrap();
    a.withdraw(30).unwrap();
    let refused = a.withdraw(500);
    println!("{} {:?} {:?}", a.balance(), a.history(), refused);
}
```

```output for=private-by-default
70 [100, -30] Err("insufficient funds")
```

```rust fails expect=error:E0616 id=private-field
mod bank {
    pub struct Account {
        balance: i64,
    }

    impl Account {
        pub fn new() -> Self {
            Account { balance: 0 }
        }
    }
}

fn main() {
    let mut a = bank::Account::new();
    a.balance = 1_000_000; // outside mod bank
}
```

- Verdict: native. Items and fields are private by default, and `pub` makes them visible outside their module. Privacy is per module: the code of `mod bank` can use the fields of `Account`, and code outside it cannot, so outside code cannot build an `Account` with a struct literal either and must call `new`. `pub(crate)` and `pub(super)` give levels in between. A method that returns `&[i64]` lends a read-only view, and the borrow checker stops the caller from changing the account while that view is alive.
- The failing block assigns the private field `balance` from `main`, outside the module, and the compiler reports error E0616.
