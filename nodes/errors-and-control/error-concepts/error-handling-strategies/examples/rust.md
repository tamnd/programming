```rust run id=strategy-rust
fn divide(a: i32, b: i32) -> Result<i32, String> {
    if b == 0 {
        return Err("division by zero".to_string());
    }
    Ok(a / b)
}

fn main() {
    for (a, b) in [(10, 2), (10, 0)] {
        match divide(a, b) {
            Ok(q) => println!("{a} / {b} = {q}"),
            Err(e) => println!("{a} / {b}: error: {e}"),
        }
    }
}
```

```output for=strategy-rust
10 / 2 = 5
10 / 0: error: division by zero
```

- Verdict: native. Strategy: error values of the type `Result<T, E>` for recoverable errors, and `panic!` for bugs. The type of the function shows that it can fail, and the caller must take the value out of the `Result` before it can use it.
