```rust run id=function-object-rust
fn main() {
    let factor = 3;
    let mut calls = 0;
    let mut triple = |x: i32| {
        calls += 1;
        x * factor
    };

    println!("{}", triple(5));
    let out: Vec<String> = [1, 2, 3].iter().map(|&x| triple(x).to_string()).collect();
    println!("{}", out.join(" "));
    println!("{}", calls);
}
```

```output for=function-object-rust
15
3 6 9
4
```

- Verdict: native. A Rust closure is a function object that the compiler makes. This closure changes the captured variable `calls`, so it implements `FnMut` and the binding `triple` must be `mut`.
