```rust run id=ops-traits-and-question-mark
use std::ops::{Add, Neg};

#[derive(Debug, Clone, Copy, PartialEq, PartialOrd)]
struct Money(i64);

// An operator is a trait: a + b calls Add::add(a, b).
impl Add for Money {
    type Output = Money;
    fn add(self, o: Money) -> Money { Money(self.0 + o.0) }
}

impl Neg for Money {
    type Output = Money;
    fn neg(self) -> Money { Money(-self.0) }
}

fn parse(s: &str) -> Result<i32, std::num::ParseIntError> {
    let n = s.parse::<i32>()?;  // ? returns the error early
    Ok(n * 2)
}

fn main() {
    let a = Money(150);
    let b = Money(275);
    println!("{:?} {:?} {}", a + b, -a, a < b);

    // Integer / truncates toward zero; rem_euclid gives a result that is not negative.
    println!("{} {} {}", -7 / 2, -7 % 2, (-7i32).rem_euclid(2));

    println!("{:?} {}", parse("21"), parse("x").is_err());

    // .. and ..= make ranges.
    let v: Vec<i32> = (1..=4).collect();
    println!("{:?} {:?}", v, &v[1..3]);
}
```

```output for=ops-traits-and-question-mark
Money(425) Money(-150) true
-3 -1 1
Ok(42) true
[1, 2, 3, 4] [2, 3]
```

- Verdict: native. An operator is a trait in `std::ops`, so `Money` implements `Add` for `+` and `Neg` for unary `-`, and it derives `PartialOrd` for `<`. Integer `/` truncates toward zero, and `rem_euclid` gives a remainder that is not negative. `?` returns an error early. `..` and `..=` make ranges.
