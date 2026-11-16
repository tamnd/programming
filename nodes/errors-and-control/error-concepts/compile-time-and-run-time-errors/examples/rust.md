```rust run id=when-rust
fn main() {
    let items = vec![10, 20, 30];
    for i in [1, 5] {
        match items.get(i) {
            Some(v) => println!("items[{i}] = {v}"),
            None => println!("items[{i}]: no item (get returned None)"),
        }
    }
}
```

```output for=when-rust
items[1] = 20
items[5]: no item (get returned None)
```

- Verdict: native. The Rust compiler finds syntax and type errors, and the borrow checker finds a reference that breaks the borrowing rules. An index past the end is found at run time: `items[i]` panics, and `items.get(i)` returns `None`, so the type makes the caller handle the missing item.
