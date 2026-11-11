```rust run id=traits
trait Account {
    fn owner(&self) -> &str;
    fn balance(&self) -> i64;
    fn describe(&self) -> String {
        // a default method: shared code that each type can override
        format!("account of {}: {}", self.owner(), self.balance())
    }
}

trait Interest: Account {
    // a supertrait: a type with Interest must also have Account
    fn rate_percent(&self) -> i64;
    fn interest(&self) -> i64 {
        self.balance() * self.rate_percent() / 100
    }
}

struct Basic {
    owner: String,
    balance: i64,
}

struct Savings {
    basic: Basic, // composition in place of a base class
    rate: i64,
}

impl Account for Basic {
    fn owner(&self) -> &str {
        &self.owner
    }
    fn balance(&self) -> i64 {
        self.balance
    }
}

impl Account for Savings {
    fn owner(&self) -> &str {
        self.basic.owner() // forwarding by hand
    }
    fn balance(&self) -> i64 {
        self.basic.balance()
    }
    fn describe(&self) -> String {
        format!("savings of {}: {}", self.owner(), self.balance())
    }
}

impl Interest for Savings {
    fn rate_percent(&self) -> i64 {
        self.rate
    }
}

fn main() {
    let b = Basic { owner: "bo".into(), balance: 50 };
    let s = Savings { basic: Basic { owner: "ana".into(), balance: 1000 }, rate: 2 };
    let all: Vec<&dyn Account> = vec![&b, &s];
    for a in &all {
        println!("{}", a.describe());
    }
    println!("interest {}", s.interest());
}
```

```output for=traits
account of bo: 50
savings of ana: 1000
interest 20
```

- Verdict: absent. Rust has no inheritance of structs. The Rust Book gives the two reasons to use inheritance, the reuse of code and the use of a child type where the parent type is expected, and shows the replacements: default methods in traits give the reuse, and trait objects and generics give the substitution. A supertrait, `trait Interest: Account`, requires one trait for another, which is inheritance between interfaces, not between data.
- Data is shared by composition: `Savings` holds a `Basic` and forwards to it. The forwarding is explicit code, which is the cost of this design, and it keeps each type in control of the methods that it exposes.
