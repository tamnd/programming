```rust run id=procedure-rust
fn greet(name: &str) {
    println!("Hello, {}!", name);
}

fn main() {
    greet("Ada");
    let result = greet("Alan");
    println!("{:?}", result);
}
```

```output for=procedure-rust
Hello, Ada!
Hello, Alan!
()
```

- Verdict: native. A function with no output type returns the unit value `()`, so a procedure is a function that returns unit.
