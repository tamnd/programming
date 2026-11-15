```rust run id=date-range-value
use std::collections::HashMap;
use std::fmt;

// A calendar day. The derived order compares the year, then the month, then the day.
#[derive(Clone, Copy, PartialEq, Eq, Hash, PartialOrd, Ord, Debug)]
struct Day {
    y: i32,
    m: u32,
    d: u32,
}

fn day(y: i32, m: u32, d: u32) -> Day {
    Day { y, m, d }
}

impl fmt::Display for Day {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{:04}-{:02}-{:02}", self.y, self.m, self.d)
    }
}

// Private fields and no setters: a DateRange cannot change after it is created.
#[derive(Clone, Copy, PartialEq, Eq, Hash, Debug)]
struct DateRange {
    start: Day,
    end: Day,
}

impl DateRange {
    fn new(start: Day, end: Day) -> Result<DateRange, &'static str> {
        if end < start {
            return Err("the end is before the start");
        }
        Ok(DateRange { start, end })
    }

    fn contains(&self, t: Day) -> bool {
        self.start <= t && t <= self.end
    }

    // A new value; self does not change.
    fn extend_to(&self, end: Day) -> Result<DateRange, &'static str> {
        DateRange::new(self.start, end)
    }
}

impl fmt::Display for DateRange {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}..{}", self.start, self.end)
    }
}

fn yn(b: bool) -> &'static str {
    if b { "yes" } else { "no" }
}

fn main() {
    let a = DateRange::new(day(2026, 3, 1), day(2026, 3, 10)).unwrap();
    let b = DateRange::new(day(2026, 3, 1), day(2026, 3, 10)).unwrap();
    println!("a equals b: {}", yn(a == b));
    println!("a and b are the same object: {}", yn(std::ptr::eq(&a, &b)));

    let c = a.extend_to(day(2026, 3, 15)).unwrap();
    println!("a: {}", a);
    println!("c: {}", c);

    let rooms = HashMap::from([(a, "room 4")]);
    println!("the room for b: {}", rooms[&b]);
    println!("a contains 2026-03-05: {}", yn(a.contains(day(2026, 3, 5))));
    println!("a contains 2026-03-12: {}", yn(a.contains(day(2026, 3, 12))));

    if let Err(e) = DateRange::new(day(2026, 3, 10), day(2026, 3, 1)) {
        println!("error: {}", e);
    }
}
```

```output
a equals b: yes
a and b are the same object: no
a: 2026-03-01..2026-03-10
c: 2026-03-01..2026-03-15
the room for b: room 4
a contains 2026-03-05: yes
a contains 2026-03-12: no
error: the end is before the start
```

- Verdict: native. `#[derive(PartialEq, Eq, Hash)]` gives equality and a hash from the fields. `Copy` makes each assignment a copy, and a value without `mut` cannot change.
- `std::ptr::eq` compares addresses, so it shows that `a` and `b` are two objects with equal values.
