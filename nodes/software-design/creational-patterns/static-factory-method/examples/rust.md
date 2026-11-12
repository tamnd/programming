```rust run id=duration
use std::fmt;
use std::rc::Rc;

pub struct Duration {
    seconds: u64, // private, so other modules must use the functions below
}

thread_local! {
    static ZERO: Rc<Duration> = Rc::new(Duration { seconds: 0 });
}

impl Duration {
    pub fn from_secs(seconds: u64) -> Rc<Duration> {
        if seconds == 0 {
            return ZERO.with(Rc::clone); // the same instance every time
        }
        Rc::new(Duration { seconds })
    }

    pub fn from_mins(minutes: u64) -> Rc<Duration> {
        Duration::from_secs(minutes * 60)
    }

    pub fn parse(text: &str) -> Result<Rc<Duration>, String> {
        let (number, unit) = text.split_at(text.len().saturating_sub(1));
        let n: u64 = number.parse().map_err(|_| format!("bad number in {text:?}"))?;
        match unit {
            "s" => Ok(Duration::from_secs(n)),
            "m" => Ok(Duration::from_mins(n)),
            _ => Err(format!("bad unit in {text:?}")),
        }
    }
}

impl fmt::Display for Duration {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{} s", self.seconds)
    }
}

fn main() {
    println!("from_mins(2) = {}", Duration::from_mins(2));
    println!("from_secs(90) = {}", Duration::from_secs(90));
    println!("zero is cached: {}", Rc::ptr_eq(&Duration::from_secs(0), &Duration::from_mins(0)));
    match Duration::parse("2m") {
        Ok(d) => println!("parse(\"2m\") = {d}"),
        Err(e) => println!("parse(\"2m\") failed: {e}"),
    }
    match Duration::parse("2h") {
        Ok(d) => println!("parse(\"2h\") = {d}"),
        Err(e) => println!("parse(\"2h\") failed: {e}"),
    }
}
```

```output
from_mins(2) = 120 s
from_secs(90) = 90 s
zero is cached: true
parse("2m") = 120 s
parse("2h") failed: bad unit in "2h"
```

- Verdict: idiom. Rust has no constructors in the language. The API Guidelines say that constructors are static, inherent methods, and `new` is the usual name of the main one.
- Other names follow the same rule: `with_capacity`, `from_secs`, `from_raw_os_error`. `std::time::Duration::from_secs` is the standard version of this example.
- A struct with a private field cannot be built with a struct literal in another module, so the functions are the only way in.
- A small value is usually returned by value. The example returns `Rc<Duration>` only to show that a factory can return a shared instance.
