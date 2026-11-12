```rust run id=let-shadowing-and-patterns
fn main() {
    // let makes a new binding. A second let with the same name shadows the first,
    // and the new binding can have another type.
    let n = "42";
    let n: i32 = n.parse().unwrap();
    println!("{}", n + 1);

    // A binding is immutable unless it is declared mut.
    let mut total = 0;
    for x in [1, 2, 3] {
        total += x;
    }
    println!("{total}");

    // let b = a moves a String: the value now belongs to b, and a cannot be used.
    let a = String::from("hi");
    let b = a;
    println!("{b}");

    // A pattern binds names to parts of a value; @ binds a name and tests it.
    let point = (3, -1);
    let (x, y) = point;
    let size = match x {
        small @ 0..=9 => format!("small {small}"),
        other => format!("big {other}"),
    };
    println!("{x} {y} {size}");

    // A move closure copies or moves the value into the closure when the closure is made.
    let mut k = 1;
    let show = move || k * 10;
    k = 5;
    println!("{} {}", show(), k);
}
```

```output for=let-shadowing-and-patterns
43
6
hi
3 -1 small 3
10 5
```

- Verdict: native. `let` makes a new binding, and a second `let` with the same name shadows the first, even with another type. A binding is immutable unless it is `mut`. `let b = a` moves the `String`, so `a` cannot be used after it. Patterns bind names to the parts of a value, and `@` binds a name and tests it. A `move` closure copies `k` when it is made, so it returns `10`.
