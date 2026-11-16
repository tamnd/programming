```rust run id=anonymous-function-rust
fn main() {
    let mut words = vec!["house", "car", "bike"];
    words.sort_by_key(|w| w.len());
    println!("{}", words.join(" "));

    let squares: Vec<String> = [1, 2, 3].iter().map(|x| (x * x).to_string()).collect();
    println!("{}", squares.join(" "));

    let n = 5;
    let add = |x| x + n;
    println!("{}", add(10));
}
```

```output for=anonymous-function-rust
car bike house
1 4 9
15
```

- Verdict: native. Closures such as `|w| w.len()` need no type annotations. The closure `add` borrows `n`.
