```rust run id=match-as-switch
fn days_in(month: u32) -> u32 {
    match month {
        2 => 28,
        4 | 6 | 9 | 11 => 30,
        _ => 31,
    }
}

fn size(n: u32) -> &'static str {
    match n {
        0..=9 => "small",
        10..=99 => "medium",
        _ => "large",
    }
}

fn main() {
    println!("{} {} {}", days_in(2), days_in(4), days_in(7));
    println!("{} {} {}", size(5), size(50), size(500));
}
```

```output for=match-as-switch
28 30 31
small medium large
```

- Verdict: native. Rust has no `switch`. A `match` on literal values does the same job, with `|` for several values and `0..=9` for a range. A `match` must cover every value of its type, so each `match` here ends with `_`.
