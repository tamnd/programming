```rust run id=derive-standard-traits
use std::collections::HashMap;
use std::fmt;

#[derive(Debug, Clone, PartialEq, Eq, Hash, PartialOrd, Ord)]
struct Money {
    currency: String,
    cents: i64,
}

impl fmt::Display for Money {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}.{:02} {}", self.cents / 100, self.cents % 100, self.currency)
    }
}

fn main() {
    let a = Money { currency: "EUR".to_string(), cents: 1250 };
    let b = a.clone();
    println!("{}", a == b);
    let mut names = HashMap::new();
    names.insert(a.clone(), "lunch");
    println!("{} {}", names[&b], a);
    println!("{:?}", a);
    let mut list = vec![Money { currency: "USD".to_string(), cents: 5 }, a];
    list.sort();
    let shown: Vec<String> = list.iter().map(|m| m.to_string()).collect();
    println!("{}", shown.join(", "));
}
```

```output for=derive-standard-traits
true
lunch 12.50 EUR
Money { currency: "EUR", cents: 1250 }
12.50 EUR, 0.05 USD
```

- Verdict: stdlib. Each standard method is a trait in the standard library: `PartialEq` and `Eq` for `==`, `Hash` for hash maps, `PartialOrd` and `Ord` for `<` and sorting, `Debug` for `{:?}`, `Display` for `{}`, and `Clone` for copies.
- `#[derive]` writes the code for most of them from the fields. `Display` cannot be derived, so the type implements it by hand, and `to_string` then comes from `Display`.
- The derived `Ord` compares the fields in the order of their declaration, so `currency` comes first here.
