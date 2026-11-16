```rust run id=error-rust
fn main() {
    for text in ["42", "x42"] {
        match text.parse::<i32>() {
            Ok(n) => println!("{text} -> {n}"),
            Err(e) => println!("{text} -> error ({:?})", e.kind()),
        }
    }
}
```

```output for=error-rust
42 -> 42
x42 -> error (InvalidDigit)
```

- Verdict: native. Rust reports a recoverable error with the type `Result<T, E>`, and stops on a bug with `panic!`. `parse` returns `Err` with a `ParseIntError`, and its `kind` method names the cause.
