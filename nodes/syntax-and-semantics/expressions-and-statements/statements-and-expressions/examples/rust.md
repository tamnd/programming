```rust run id=blocks-are-expressions
fn label(score: u32) -> &'static str {
    match score {
        90.. => "A",
        50..=89 => "pass",
        _ => "fail",
    } // the match is the last expression, so it is the return value
}

fn main() {
    let labels: Vec<_> = [95, 70, 30].into_iter().map(label).collect();
    println!("{labels:?}");

    let area = {
        let w = 3;
        let h = 4;
        w * h // no semicolon: the block has this value
    };
    let sign = if area > 10 { "big" } else { "small" };
    println!("{area} {sign}");

    let data = [3, 1, 4, 0, 5];
    let mut i = 0;
    let total = loop {
        if data[i] == 0 {
            break data[..i].iter().sum::<i32>(); // break with a value
        }
        i += 1;
    };
    println!("{total}");
    let unit: () = (); // a statement has the value of type ()
    println!("{unit:?}");
}
```

```output for=blocks-are-expressions
["A", "pass", "fail"]
12 big
8
()
```

```rust fails expect=error:E0308 id=semicolon-ends-the-value
fn area(w: u32, h: u32) -> u32 {
    w * h; // the semicolon makes this a statement, so the body has no value
}

fn main() {
    println!("{}", area(3, 4));
}
```

- Verdict: native. Rust is an expression language. Blocks, `if`, `match` and `loop` are expressions with values, the last expression of a block without a semicolon is the value of the block, and `break` can carry a value out of a `loop`. A semicolon turns an expression into a statement, and a statement has the unit value `()`.
- The fails block ends the body of `area` with `w * h;`. The semicolon makes the multiplication a statement, so the body has the value `()`, which does not match the return type `u32`, and the compiler reports mismatched types and suggests removing the semicolon.
