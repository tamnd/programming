```rust run id=const
const MAX_SIZE: usize = 10;
const SECONDS_PER_DAY: u32 = 24 * 60 * 60;

fn main() {
    let names = vec!["Ada", "Bob"];
    let count = names.len();
    println!("{} {} {}", MAX_SIZE, SECONDS_PER_DAY, count);
}
```

```output for=const
10 86400 2
```

- Verdict: native. A `const` item needs a type, and its value is computed at compile time, so `24 * 60 * 60` costs nothing at run time. A `let` binding without `mut`, such as `count`, is a run-time constant: the value comes from the program, and the compiler rejects a second assignment.
