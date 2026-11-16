```rust run id=match-guards-rust
fn describe(n: i32) -> &'static str {
    match n {
        0 => "zero",
        k if k < 0 => "negative",
        k if k < 10 && k % 2 == 0 => "small even",
        k if k < 10 => "small odd",
        _ => "large",
    }
}

fn main() {
    let words: Vec<&str> = [-5, 0, 4, 7, 12].iter().map(|&n| describe(n)).collect();
    println!("{}", words.join(", "));
}
```

```output for=match-guards-rust
negative, zero, small even, small odd, large
```

- Verdict: native. A match arm can have `if` and a condition after its pattern. The compiler does not count a guarded arm when it checks that the match is exhaustive, so the last arm `_` has no guard.
