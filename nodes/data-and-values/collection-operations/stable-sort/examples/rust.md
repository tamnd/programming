```rust run id=sort-by-key-is-stable
fn names(s: &[(&str, &str, u32)]) -> Vec<String> {
    s.iter().map(|p| p.0.to_string()).collect()
}

fn main() {
    let all = vec![("ann", "ops", 3), ("bob", "dev", 2), ("cy", "ops", 2), ("di", "dev", 3), ("ed", "ops", 2)];

    // sort_by_key is stable: equal teams keep the name order.
    let mut s = all.clone();
    s.sort_by_key(|p| p.1);
    println!("{:?}", names(&s));

    // Two stable passes: grade first, then team.
    let mut s = all.clone();
    s.sort_by_key(|p| p.2);
    s.sort_by_key(|p| p.1);
    println!("{:?}", names(&s));

    // sort_unstable_by_key may reorder equal items. A key that includes the
    // original position makes the result the same as a stable sort.
    let mut s: Vec<(usize, _)> = all.iter().copied().enumerate().collect();
    s.sort_unstable_by_key(|&(i, p)| (p.1, i));
    println!("{:?}", s.iter().map(|&(_, p)| p.0).collect::<Vec<_>>());
}
```

```output
["bob", "di", "ann", "cy", "ed"]
["bob", "di", "cy", "ed", "ann"]
["bob", "di", "ann", "cy", "ed"]
```

- Verdict: native. `sort_by_key` is stable, so one pass keeps the name order and two passes sort by team and then by grade. `sort_unstable_by_key` may reorder equal items, so the key includes the position from `enumerate`, and the result is the same as the stable sort.
