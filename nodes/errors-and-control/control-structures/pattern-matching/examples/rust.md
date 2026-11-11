```rust run id=points
fn describe(point: (i32, i32)) -> String {
    match point {
        (0, 0) => "origin".to_string(),
        (x, 0) => format!("on the x axis at {x}"),
        (0, y) => format!("on the y axis at {y}"),
        (x, y) if x == y => format!("on the diagonal at {x}"),
        (x, y) => format!("at {x}, {y}"),
    }
}

fn main() {
    for point in [(0, 0), (3, 0), (0, -2), (4, 4), (1, 5)] {
        println!("{:?}: {}", point, describe(point));
    }

    let (x, y) = (7, 8);
    println!("let takes apart a tuple: {x} {y}");

    let input = "42";
    let Ok(n) = input.parse::<i32>() else {
        println!("not a number");
        return;
    };
    println!("let else bound {n}");
}
```

```output for=points
(0, 0): origin
(3, 0): on the x axis at 3
(0, -2): on the y axis at -2
(4, 4): on the diagonal at 4
(1, 5): at 1, 5
let takes apart a tuple: 7 8
let else bound 42
```

- Verdict: native. `match`, `if let`, `while let`, `let`, `let else` and function parameters all take patterns.
- A `match` must be exhaustive, or it does not compile (error E0004). The compiler does not use the guards when it checks this, so the last arm here has no guard.
- `let` and function parameters accept only irrefutable patterns. `let else` accepts a refutable pattern, and its `else` block must leave the scope, here with `return`.
