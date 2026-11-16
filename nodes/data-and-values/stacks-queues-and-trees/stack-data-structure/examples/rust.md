```rust run id=stack
fn main() {
    let mut stack: Vec<&str> = Vec::new();
    for item in ["a", "b", "c"] {
        stack.push(item);
    }
    let top = stack.last();
    println!("{stack:?} top {top:?}");

    let first = stack.pop();
    let second = stack.pop();
    println!("{first:?} {second:?} {stack:?}");
    stack.pop();
    println!("pop on empty: {:?}", stack.pop());
}
```

```output for=stack
["a", "b", "c"] top Some("c")
Some("c") Some("b") ["a"]
pop on empty: None
```

- Verdict: stdlib. A `Vec` is the stack. `push` and `pop` work at its end, and `last` peeks.
- `pop` and `last` return an `Option`, so an empty stack gives `None` and not a crash.
