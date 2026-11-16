```rust run id=local-function-rust
fn sum_of_scaled(values: &[i32], factor: i32) -> i32 {
    fn scale(x: i32, factor: i32) -> i32 {
        x * factor
    }

    let mut total = 0;
    for &v in values {
        total += scale(v, factor);
    }
    total
}

fn main() {
    println!("{}", sum_of_scaled(&[1, 2, 3], 10));
    println!("{}", sum_of_scaled(&[4], 2));
}
```

```output for=local-function-rust
60
8
```

- Verdict: native. Rust allows a `fn` item in a function body, but the item cannot capture the outer variables, so `scale` takes `factor` as a parameter. A closure can capture it instead.
