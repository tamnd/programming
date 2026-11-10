```rust run id=types
use std::any::Any;

fn describe(value: &dyn Any) -> String {
    if let Some(n) = value.downcast_ref::<i32>() {
        format!("i32 {}", n + 1)
    } else if let Some(s) = value.downcast_ref::<&str>() {
        format!("&str of length {}", s.len())
    } else {
        String::from("other")
    }
}

fn main() {
    let count = 41;
    let values: [&dyn Any; 3] = [&count, &"abc", &2.5_f64];
    for value in values {
        println!("{}", describe(value));
    }
    println!("{}", values[0].is::<i64>());
}
```

```output
i32 42
&str of length 3
other
false
```

- Verdict: native. Rust is statically typed. The compiler infers `i32` for `count` and fixes it.
- `dyn Any` keeps the type of a value for a check at run time. `downcast_ref` gives `Some` for the right type and `None` for a wrong type, and `is` gives a `bool`.
- Rust does not convert between numeric types by itself. An expression that adds an `i32` and an `i64` does not compile. Write `i64::from(a)` or `a as i64`.

```rust fails expect=compile id=mix
fn main() {
    let a: i32 = 2;
    let b: i64 = 3;
    println!("{}", a + b);
}
```
