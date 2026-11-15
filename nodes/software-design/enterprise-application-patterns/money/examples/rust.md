```rust run id=money-allocate
use std::fmt;

// From ISO 4217.
fn fraction_digits(currency: &str) -> u32 {
    match currency {
        "JPY" => 0,
        _ => 2,
    }
}

// units: in the minor unit of the currency, such as cents.
#[derive(Clone, Copy, PartialEq, Eq, Hash, Debug)]
struct Money {
    units: i64,
    currency: &'static str,
}

impl Money {
    fn new(units: i64, currency: &'static str) -> Money {
        Money { units, currency }
    }

    fn plus(self, other: Money) -> Result<Money, String> {
        if self.currency != other.currency {
            return Err(format!("cannot add {} and {}", self.currency, other.currency));
        }
        Ok(Money::new(self.units + other.units, self.currency))
    }

    // Split by ratios. The sum of the parts is always the whole amount.
    fn allocate(self, ratios: &[i64]) -> Vec<Money> {
        let total: i64 = ratios.iter().sum();
        let mut parts: Vec<Money> = ratios.iter().map(|r| Money::new(self.units * r / total, self.currency)).collect();
        let left = self.units - parts.iter().map(|p| p.units).sum::<i64>();
        for p in parts.iter_mut().take(left as usize) {
            p.units += 1; // the units that are left, one each
        }
        parts
    }
}

impl fmt::Display for Money {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        let d = fraction_digits(self.currency);
        if d == 0 {
            return write!(f, "{} {}", self.units, self.currency);
        }
        let scale = 10_i64.pow(d);
        write!(f, "{}.{:0w$} {}", self.units / scale, self.units % scale, self.currency, w = d as usize)
    }
}

fn show(parts: &[Money]) -> String {
    parts.iter().map(|p| p.to_string()).collect::<Vec<_>>().join(", ")
}

fn main() {
    let a = Money::new(1000, "EUR");
    let b = Money::new(50, "EUR");
    println!("a: {}", a);
    let sum = a.plus(b).unwrap();
    println!("a + b: {}", sum);
    if let Err(e) = a.plus(Money::new(500, "JPY")) {
        println!("error: {}", e);
    }
    println!("fee: {}", Money::new(500, "JPY"));
    println!("5 cents 70/30: {}", show(&Money::new(5, "EUR").allocate(&[70, 30])));
    println!("100 EUR in 3: {}", show(&Money::new(10000, "EUR").allocate(&[1, 1, 1])));
    println!("equal: {}", if Money::new(1050, "EUR") == sum { "yes" } else { "no" });
}
```

```output
a: 10.00 EUR
a + b: 10.50 EUR
error: cannot add EUR and JPY
fee: 500 JPY
5 cents 70/30: 0.04 EUR, 0.01 EUR
100 EUR in 3: 33.34 EUR, 33.33 EUR, 33.33 EUR
equal: yes
```

- Verdict: idiom. `Money` derives `PartialEq`, `Eq` and `Hash`, and `Copy` makes it a value. The standard library has no money or decimal type.
- `take(left as usize)` gives one more unit to each of the first parts, which is the same rule as the other examples.
