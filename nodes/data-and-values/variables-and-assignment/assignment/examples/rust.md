```rust run id=let-mut
fn main() {
    let mut a = 1;
    let mut b = 2;
    (a, b) = (b, a);
    println!("{a} {b}");
    let unit = a = 10;
    println!("{a} {unit:?}");
    let mut total = 0;
    for n in [3, 4, 5] {
        total += n;
    }
    println!("{total}");
}
```

```output for=let-mut
2 1
10 ()
12
```

- Verdict: native. Only a `let mut` variable can be assigned. An assignment is an expression of the unit type `()`: `let unit = a = 10` sets `a` to 10, and `unit` gets the value `()`. So `a = b = 0` does not compile for numbers, and `if a = 0` is a type error. A tuple on the left takes the parts of a tuple on the right.
