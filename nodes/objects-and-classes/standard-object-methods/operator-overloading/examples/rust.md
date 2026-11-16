```rust run id=ops-traits
use std::iter::Sum;
use std::ops::{Add, Mul};

#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord, Hash)]
struct Money(i64);

impl Add for Money {
    type Output = Money;
    fn add(self, other: Money) -> Money {
        Money(self.0 + other.0)
    }
}

// Money * i64, with an operand of another type.
impl Mul<i64> for Money {
    type Output = Money;
    fn mul(self, k: i64) -> Money {
        Money(self.0 * k)
    }
}

impl Sum for Money {
    fn sum<I: Iterator<Item = Money>>(iter: I) -> Money {
        iter.fold(Money(0), |acc, m| acc + m)
    }
}

fn main() {
    let a = Money(150);
    let b = Money(250);
    println!("{:?}", a + b);
    println!("{} {}", a + b == Money(400), a < b);
    println!("{:?}", a * 3);
    let total: Money = vec![a, b].into_iter().sum();
    println!("{:?}", total);
}
```

```output for=ops-traits
Money(400)
true true
Money(450)
Money(400)
```

- Verdict: stdlib. Each operator has a trait in `std::ops`: `impl Add` gives `+`, and `impl Mul<i64>` gives `Money * i64`. The associated type `Output` is the type of the result.
- `==` and `<` come from `PartialEq` and `PartialOrd`, here derived from the field.
- `i64 * Money` does not compile; it needs a separate `impl Mul<Money> for i64`. `impl Sum` lets `Iterator::sum` add the values.
