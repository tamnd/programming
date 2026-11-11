```rust run id=account
struct Account {
    balance: u32,
}

impl Account {
    fn withdraw(&mut self, amount: u32) -> Result<(), String> {
        if amount == 0 || amount > self.balance {
            return Err(format!("cannot withdraw {amount} from {}", self.balance));
        }
        let old = self.balance;
        self.balance -= amount;
        debug_assert_eq!(self.balance, old - amount, "postcondition");
        Ok(())
    }
}

const _: () = assert!(u32::BITS == 32);

fn main() {
    let mut account = Account { balance: 100 };
    println!("{:?} {}", account.withdraw(30), account.balance);
    println!("{:?} {}", account.withdraw(500), account.balance);
    println!("debug assertions: {}", cfg!(debug_assertions));
}
```

```output
Ok(()) 70
Err("cannot withdraw 500 from 70") 70
debug assertions: true
```

- Verdict: native. `assert!` panics when its condition is false, in every build. `assert_eq!` and `assert_ne!` compare two values and print both on failure.
- `debug_assert!` and `debug_assert_eq!` run only in builds without optimization, unless the compiler gets `-C debug-assertions`. The expression is still type checked in every build.
- `assert!` in a `const` item runs when the compiler evaluates the constant. The last block below shows a false one.
- The type `u32` cannot hold a negative balance, so the invariant "balance is not negative" is part of the type and needs no assertion.

A failed `assert!` panics. The default hook prints the message, and the program exits with status 101.

```rust fails expect=panic id=violated
fn average(values: &[u32]) -> u32 {
    assert!(!values.is_empty(), "precondition: no values");
    values.iter().sum::<u32>() / values.len() as u32
}

fn main() {
    println!("{}", average(&[2, 4]));
    println!("{}", average(&[]));
}
```

In a `const` item, the compiler evaluates the assertion, and a false one stops the build.

```rust fails expect=compile id=const
struct Header {
    tag: u8,
    length: u32,
}

const _: () = assert!(std::mem::size_of::<Header>() == 5, "Header is 5 bytes");

fn main() {
    let h = Header { tag: 1, length: 2 };
    println!("{} {}", h.tag, h.length);
}
```
