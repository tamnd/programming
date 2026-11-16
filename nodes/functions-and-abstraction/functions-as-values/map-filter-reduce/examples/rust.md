```rust run id=map-filter-reduce-rust
fn main() {
    let squares: Vec<i32> = (1..=6).filter(|x| x % 2 == 0).map(|x| x * x).collect();
    let text: Vec<String> = squares.iter().map(|x| x.to_string()).collect();
    println!("{}", text.join(" "));
    println!("{}", squares.iter().fold(0, |acc, x| acc + x));
}
```

```output for=map-filter-reduce-rust
4 16 36
56
```

- Verdict: stdlib. The trait `Iterator` has `filter`, `map` and `fold`. The adapters are lazy, and `collect` runs them.
