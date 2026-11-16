```rust run id=recoverable-rust
fn port_or_default(text: &str, fallback: u32) -> (u32, &'static str) {
    // A bad fallback is a bug in the caller: panic.
    assert!((1..=65535).contains(&fallback), "fallback out of range");
    // Bad text is an expected error: recover.
    match text.parse::<u32>() {
        Ok(n) => (n, ""),
        Err(_) => (fallback, " (not a number)"),
    }
}

fn main() {
    for text in ["8080", "x"] {
        let (port, note) = port_or_default(text, 80);
        println!("{text} -> {port}{note}");
    }
}
```

```output for=recoverable-rust
8080 -> 8080
x -> 80 (not a number)
```

- Verdict: native. Rust has a mechanism for each kind: `Result<T, E>` for a recoverable error, and `panic!` for an unrecoverable one. `assert!` panics when its condition is false. A panic unwinds the stack of the thread and stops it.
