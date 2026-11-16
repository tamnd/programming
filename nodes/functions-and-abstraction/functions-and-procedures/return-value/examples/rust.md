```rust run id=return-value-rust
fn absolute(n: i32) -> i32 {
    if n < 0 {
        -n
    } else {
        n
    }
}

fn main() {
    println!("{}", absolute(-7));
    println!("{}", absolute(5) + 1);
}
```

```output for=return-value-rust
7
6
```

- Verdict: native. The `if` is an expression, and the function returns the value of the last expression of its body, so it needs no `return`.
