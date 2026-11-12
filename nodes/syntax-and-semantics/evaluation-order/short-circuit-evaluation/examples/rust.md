```rust run id=lazy-and-eager-defaults
fn check(name: &str, v: bool, log: &mut Vec<String>) -> bool {
    log.push(name.to_string());
    v
}

fn expensive() -> i32 {
    println!("  expensive() ran");
    42
}

fn main() {
    let mut log = Vec::new();
    // && and || skip the right operand; & on bool evaluates both.
    let a = check("a", false, &mut log) && check("b", true, &mut log);
    println!("{a} {log:?}");
    log.clear();
    let b = check("a", false, &mut log) & check("b", true, &mut log);
    println!("{b} {log:?}");

    // unwrap_or takes a value, so its argument is computed first; unwrap_or_else takes a closure.
    let x = Some(1);
    println!("unwrap_or:");
    println!("  {}", x.unwrap_or(expensive()));
    println!("unwrap_or_else:");
    println!("  {}", x.unwrap_or_else(expensive));

    // bool::then runs the closure only for true.
    let d = 0;
    println!("{:?} {:?}", (d != 0).then(|| 10 / d), (2 != 0).then(|| 10 / 2));
}
```

```output for=lazy-and-eager-defaults
false ["a"]
false ["a", "b"]
unwrap_or:
  expensive() ran
  1
unwrap_or_else:
  1
None Some(5)
```

- Verdict: native. `&&` and `||` skip the right operand, and `&` on `bool` evaluates both. `unwrap_or` takes a value, so its argument is computed even when the option has a value; `unwrap_or_else` takes a function and calls it only when needed. `bool::then` runs a closure only for `true`, so the division by zero does not run.
