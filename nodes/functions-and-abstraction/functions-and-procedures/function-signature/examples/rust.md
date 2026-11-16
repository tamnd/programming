```rust run id=function-signature-rust
fn average(a: i32, b: i32) -> f64 {
    f64::from(a + b) / 2.0
}

fn main() {
    println!("{}", average(3, 4));
}
```

```output for=function-signature-rust
3.5
```

- Verdict: native. Each parameter must have a type, and `->` gives the output type.
