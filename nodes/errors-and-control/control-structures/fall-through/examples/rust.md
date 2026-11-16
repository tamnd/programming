```rust run id=fall-through-rust
fn rights(level: u32) -> Vec<&'static str> {
    let mut out = Vec::new();
    match level {
        3 => out.push("delete"),
        2 => out.push("write"),
        1 => out.push("read"),
        _ => out.push("none"),
    }
    out
}

fn rights_cumulative(level: u32) -> Vec<&'static str> {
    let all = ["read", "write", "delete"];
    let n = (level as usize).min(all.len());
    let mut out: Vec<&str> = all[..n].to_vec();
    out.reverse();
    out
}

fn main() {
    println!("{:?} {:?}", rights(3), rights_cumulative(3));
    println!("{:?} {:?}", rights(1), rights_cumulative(1));
}
```

```output for=fall-through-rust
["delete"] ["delete", "write", "read"]
["read"] ["read"]
```

- Verdict: absent. A `match` runs the code of the first arm that matches, and then the `match` ends. Rust has no fall-through and no statement for it, so code that needs the effect of several arms computes it in another way, as `rights_cumulative` does.
