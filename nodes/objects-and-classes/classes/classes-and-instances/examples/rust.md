```rust run id=struct-and-impl
use std::fmt;

// The data: a struct with private fields.
pub struct Account {
    owner: String,
    balance: i64,
}

// The behavior: an impl block, separate from the data.
impl Account {
    pub const RATE: f64 = 0.02; // an associated constant, one for the type

    pub fn new(owner: &str, balance: i64) -> Self {
        Account {
            owner: owner.to_string(),
            balance,
        }
    }

    pub fn deposit(&mut self, amount: i64) {
        self.balance += amount; // &mut self: the method changes the account
    }

    pub fn interest(&self) -> f64 {
        self.balance as f64 * Self::RATE // &self: the method only reads
    }
}

impl fmt::Display for Account {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Account({}, {})", self.owner, self.balance)
    }
}

fn main() {
    let mut a = Account::new("ana", 100);
    let b = Account::new("bo", 0);
    a.deposit(50);
    println!("{} {}", a, b);
    println!("{}", a.interest());
    Account::deposit(&mut a, 10); // the same call, written as a function
    println!("{}", a);
}
```

```output for=struct-and-impl
Account(ana, 150) Account(bo, 0)
3
Account(ana, 160)
```

```rust fails expect=error:E0596 id=method-needs-mut
struct Account {
    balance: i64,
}

impl Account {
    fn deposit(&mut self, amount: i64) {
        self.balance += amount;
    }
}

fn main() {
    let a = Account { balance: 100 }; // not declared mut
    a.deposit(50);
    println!("{}", a.balance);
}
```

- Verdict: idiom. Rust has no classes. A `struct` holds the data and one or more `impl` blocks add methods and associated constants. The first parameter of a method states what the method may do: `&self` reads, `&mut self` changes, and `self` takes ownership. `a.deposit(50)` is short for `Account::deposit(&mut a, 50)`. Traits, implemented in their own `impl` blocks, give shared behavior such as `Display`. Fields are private to the module unless marked `pub`.
- The failing block calls a method that takes `&mut self` on a variable that is not declared `mut`, and the compiler reports error E0596. Declare the variable with `let mut`.
