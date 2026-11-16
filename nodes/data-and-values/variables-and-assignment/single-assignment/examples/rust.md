```rust run id=let-and-shadow
fn main() {
    let x = 5;
    let x = x + 1;
    {
        let x = x * 2;
        println!("{x}");
    }
    println!("{x}");

    let spaces = "   ";
    let spaces = spaces.len();
    println!("{spaces}");
}
```

```output for=let-and-shadow
12
6
3
```

- Verdict: native. A `let` variable is immutable unless it has `mut`, and an assignment to it does not compile. Shadowing makes a new variable with the same name, so each `x` is assigned once. The new variable can have another type: the second `spaces` is a number.
