```rust run id=function-rust
fn rectangle_area(width: i32, height: i32) -> i32 {
    width * height
}

fn greeting(name: &str) -> String {
    format!("Hello, {}!", name)
}

fn main() {
    println!("{}", rectangle_area(3, 4));
    println!("{}", greeting("Ada"));
}
```

```output for=function-rust
12
Hello, Ada!
```

- Verdict: native. `fn` declares a function. The last expression of the body, with no semicolon, is the result.
