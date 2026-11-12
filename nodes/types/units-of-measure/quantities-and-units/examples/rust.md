```rust run id=newtypes-and-duration
use std::ops::{Add, Div};
use std::time::Duration;

#[derive(Debug, Clone, Copy, PartialEq)]
struct Metres(f64);
#[derive(Debug, Clone, Copy, PartialEq)]
struct Seconds(f64);
#[derive(Debug, Clone, Copy, PartialEq)]
struct MetresPerSecond(f64);

impl Add for Metres {
    type Output = Metres;
    fn add(self, o: Metres) -> Metres { Metres(self.0 + o.0) }
}

impl Div<Seconds> for Metres {
    type Output = MetresPerSecond;
    fn div(self, t: Seconds) -> MetresPerSecond { MetresPerSecond(self.0 / t.0) }
}

fn main() {
    let timeout = Duration::from_secs(90) + Duration::from_millis(500);
    println!("{:?} = {} ms", timeout, timeout.as_millis());
    println!("2 min / 30 s = {}", Duration::from_secs(120).div_duration_f64(Duration::from_secs(30)));
    let trip = Metres(5000.0) + Metres(304.8);
    println!("{:?} / {:?} = {:?}", trip, Seconds(1800.0), trip / Seconds(1800.0));
    println!("checked_sub: {:?}", Duration::from_secs(1).checked_sub(timeout));
}
```

```output for=newtypes-and-duration
90.5s = 90500 ms
2 min / 30 s = 4
Metres(5304.8) / Seconds(1800.0) = MetresPerSecond(2.947111111111111)
checked_sub: None
```

```rust fails expect=error:E0308 id=metres-plus-seconds
use std::ops::Add;

struct Metres(f64);
struct Seconds(f64);

impl Add for Metres {
    type Output = Metres;
    fn add(self, o: Metres) -> Metres { Metres(self.0 + o.0) }
}

fn main() {
    let total = Metres(100.0) + Seconds(9.58);
    println!("{}", total.0);
}
```

- Verdict: library. `std::time::Duration` is the unit type for time; it cannot be negative, so `checked_sub` returns `None` for a negative result. Newtypes with `Add` and `Div` implementations give each unit its own type and state which products and quotients exist. The fails block adds `Metres` and `Seconds`, and the compiler reports E0308. The uom crate checks dimensions for all SI quantities at compile time with no run-time cost.
