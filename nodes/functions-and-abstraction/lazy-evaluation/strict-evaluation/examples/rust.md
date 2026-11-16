```rust run id=strict
fn trace<T>(label: &str, value: T) -> T {
    println!("evaluating {label}");
    value
}

fn pick(flag: bool, left: i32, right: i32) -> i32 {
    println!("inside pick");
    if flag { left } else { right }
}

fn main() {
    println!("{}", pick(trace("flag", true), trace("left", 1), trace("right", 2)));
    println!("{}", trace("x", false) && trace("y", true));
    let fallback: Option<i32> = None;
    println!("{}", fallback.unwrap_or_else(|| trace("closure", 7)));
}
```

```output for=strict
evaluating flag
evaluating left
evaluating right
inside pick
1
evaluating x
false
evaluating closure
7
```

- Verdict: native. The Rust Reference states that the operands of a call are evaluated before the call, left to right as written.
- The standard library pairs eager and lazy forms: `unwrap_or(x)` takes a value that is always computed, and `unwrap_or_else(|| ...)` takes a closure that runs only when needed.
