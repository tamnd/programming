```rust run id=labels
fn main() {
    let grid = [[3, 1], [4, -1], [-5, 9]];
    let found = 'search: {
        for (r, row) in grid.iter().enumerate() {
            for (c, &value) in row.iter().enumerate() {
                if value < 0 {
                    break 'search Some((r, c));
                }
            }
        }
        None
    };
    println!("{found:?}");

    let mut n = 27u64;
    let mut count = 0;
    let steps = loop {
        if n == 1 {
            break count;
        }
        n = if n % 2 == 0 { n / 2 } else { 3 * n + 1 };
        count += 1;
    };
    println!("{steps}");
}
```

```output for=labels
Some((1, 1))
111
```

- Verdict: native. Rust has no `goto`. Loops and blocks can have labels, and `break 'label value` leaves a labeled block or loop with a value.
- `loop` is an expression, and `break` gives it a value.
- `?` returns early from a function with an error, and `Drop` runs cleanup on every exit.
