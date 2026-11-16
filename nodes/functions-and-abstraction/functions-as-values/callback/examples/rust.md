```rust run id=callback-rust
fn repeat(n: i32, mut callback: impl FnMut(i32)) {
    for i in 1..=n {
        callback(i);
    }
}

fn tick(i: i32) {
    println!("tick {}", i);
}

fn main() {
    let mut xs = vec![3, 1, 2];
    xs.sort_by(|a, b| b.cmp(a));
    let out: Vec<String> = xs.iter().map(|x| x.to_string()).collect();
    println!("{}", out.join(" "));
    repeat(2, tick);
}
```

```output for=callback-rust
3 2 1
tick 1
tick 2
```

- Verdict: native. `sort_by` takes a closure that compares two items. `repeat` takes any type that implements `FnMut(i32)`, and a function such as `tick` implements it too.
