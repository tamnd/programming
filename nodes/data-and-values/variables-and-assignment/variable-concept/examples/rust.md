```rust run id=let-and-mut
fn main() {
    let x = 5;
    let mut y = x;
    y += 1;
    let x = "five";
    println!("{x} {y}");
}
```

```output for=let-and-mut
five 6
```

- Verdict: native. `let` declares a variable that cannot change, so `x = 6` after `let x = 5` does not compile. `let mut` allows changes. A second `let x` declares a new variable with the same name, which shadows the first one, and it can have another type.
