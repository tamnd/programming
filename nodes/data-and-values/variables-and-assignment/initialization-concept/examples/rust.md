```rust run id=deferred-init
fn main() {
    let limit: i32;
    let big = std::env::args().count() > 5;
    if big {
        limit = 100;
    } else {
        limit = 10;
    }
    println!("{limit}");

    let items: Vec<i32> = Vec::new();
    let zero: i32 = Default::default();
    println!("{} {}", items.len(), zero);
}
```

```output for=deferred-init
10
0 0
```

- Verdict: native. A `let` without a value is allowed, and the compiler checks that each path gives the variable a value before the read. A read that some path does not cover is a compile error. Rust has no implicit default value: `Default::default()` gives one when the program asks for it.
