```rust run id=refutable-rust
fn main() {
    for line in ["width=80", "oops", "height=24"] {
        // A refutable pattern: split_once can return None.
        let Some(pair) = line.split_once('=') else {
            println!("skip: {line}");
            continue;
        };
        // An irrefutable pattern: each pair has two parts.
        let (key, value) = pair;
        println!("{key} -> {value}");
    }
}
```

```output for=refutable-rust
width -> 80
skip: oops
height -> 24
```

- Verdict: native. A plain `let` takes only an irrefutable pattern, such as the tuple pattern `(key, value)`. The pattern `Some(pair)` is refutable, so it needs `let...else`, `if let` or `match`. The `else` block must diverge, here with `continue`.
