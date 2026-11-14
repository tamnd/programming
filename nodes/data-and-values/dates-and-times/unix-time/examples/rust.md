```rust run id=unix-epoch
use std::time::{Duration, SystemTime, UNIX_EPOCH};

fn main() {
    // A SystemTime is the epoch plus a Duration.
    let t = UNIX_EPOCH + Duration::from_secs(1_709_985_600);
    let d = t.duration_since(UNIX_EPOCH).unwrap();
    println!("{} {}", d.as_secs(), d.as_millis());

    // Every day has 86400 seconds, so a division gives days and the time of day.
    println!("{} {}", d.as_secs() / 86_400, d.as_secs() % 86_400);

    // duration_since returns an error for a time before the epoch.
    let before = UNIX_EPOCH - Duration::from_secs(1);
    println!("{}", before.duration_since(UNIX_EPOCH).is_err());

    let now = SystemTime::now().duration_since(UNIX_EPOCH).unwrap();
    println!("{}", now.as_secs() > 1_700_000_000);

    // A 32-bit count has no next second after i32::MAX.
    println!("{:?}", i32::MAX.checked_add(1));
}
```

```output
1709985600 1709985600000
19791 43200
true
true
None
```

- Verdict: stdlib. SystemTime::now and the constant UNIX_EPOCH give a Unix time as a Duration through duration_since, which returns an error for a time before the epoch. The standard library has no conversion to calendar fields; the crates chrono, time and jiff have it.
