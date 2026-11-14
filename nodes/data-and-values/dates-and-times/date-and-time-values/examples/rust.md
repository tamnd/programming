```rust run id=std-time
use std::time::{Duration, Instant, SystemTime, UNIX_EPOCH};

fn main() {
    // A SystemTime is a reading of the system clock. It has no calendar fields.
    let t = UNIX_EPOCH + Duration::from_secs(1_709_985_600);
    println!("{}", t.duration_since(UNIX_EPOCH).unwrap().as_secs());

    // duration_since returns an error when the other time is later.
    println!("{}", UNIX_EPOCH.duration_since(t).is_err());
    println!("{}", SystemTime::now() > t);

    // A Duration is an amount of time.
    let d = Duration::from_secs(90 * 60);
    println!("{:?} {}", d, d.as_secs_f64());

    // An Instant is for elapsed time only. It has no epoch and no date.
    let start = Instant::now();
    println!("{}", start.elapsed() < Duration::from_secs(60));
}
```

```output
1709985600
true
true
5400s 5400
true
```

- Verdict: stdlib. std::time has SystemTime, Instant and Duration, but no calendar dates, no time zones and no formatting. Programs use a crate such as chrono, time or jiff for dates.
