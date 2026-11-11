```rust run id=new-and-default
#[derive(Debug)]
struct Range {
    lo: i32,
    hi: i32,
}

impl Range {
    // No constructors in the language: `new` is an ordinary function by convention.
    fn new(lo: i32, hi: i32) -> Result<Range, String> {
        if lo > hi {
            return Err(format!("empty range {lo}..{hi}"));
        }
        Ok(Range { lo, hi }) // a struct literal must give every field
    }

    fn size(&self) -> i32 {
        self.hi - self.lo + 1
    }
}

#[derive(Default)]
struct Config {
    name: String,  // Default gives ""
    retries: u32,  // 0
    verbose: bool, // false
}

fn main() {
    println!("{:?}", Range::new(3, 7).map(|r| r.size()));
    println!("{:?}", Range::new(9, 1));
    let c = Config {
        retries: 3,
        ..Default::default()
    }; // the rest from Default
    println!("{:?} {} {}", c.name, c.retries, c.verbose);
}
```

```output for=new-and-default
Ok(5)
Err("empty range 9..1")
"" 3 false
```

```rust fails expect=error:E0063 id=missing-field
struct Range {
    lo: i32,
    hi: i32,
}

fn main() {
    let r = Range { lo: 3 }; // hi is missing
    println!("{}", r.lo);
}
```

- Verdict: idiom. Rust has no constructors. A struct literal creates a value and must give every field, so no value has a field that was never set. By convention, an associated function `new` checks the arguments and returns the value or a `Result`. The `Default` trait gives a value with default fields, and the update syntax `..Default::default()` fills the fields that a literal does not name. Private fields stop code in other modules from writing a literal at all, so `new` becomes the only way in.
- The failing block writes a struct literal without the field `hi`, and the compiler reports error E0063.
