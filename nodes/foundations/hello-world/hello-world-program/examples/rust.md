```rust run id=fn-main-and-println
fn main() {
    println!("Hello, world!");
    let name = "world";
    println!("{name} has {} letters", name.len());
    println!("arguments: {}", std::env::args().count() - 1);
}
```

```output for=fn-main-and-println
Hello, world!
world has 5 letters
arguments: 0
```

```rust fails expect=error:E0423 id=println-without-bang
fn main() {
    println("Hello, world!"); // println is a macro, so the call needs a !
}
```

- Verdict: native. A Rust program starts at `fn main`. `println!` is a macro, not a function, and the compiler checks its format string and arguments at compile time; `{name}` reads a variable by name. `std::env::args` gives the arguments, and `main` can return a `Result` so that an error gives a status other than 0.
- The fails block calls `println` without the `!`. The name exists, but as a macro, so the compiler rejects the call with E0423 and suggests `println!`.
