```rust run id=callbacks
use std::collections::VecDeque;

fn for_each(items: &[&str], callback: impl Fn(&str)) {
    for item in items {
        callback(item); // a blocking callback
    }
}

fn main() {
    for_each(&["hello", "world"], |w| println!("{}", w.to_uppercase()));

    let mut pending: VecDeque<Box<dyn FnOnce()>> = VecDeque::new();
    pending.push_back(Box::new(|| println!("saved"))); // a deferred callback
    println!("request sent");
    while let Some(f) = pending.pop_front() {
        f();
    }
}
```

```output for=callbacks
HELLO
WORLD
request sent
saved
```

- Verdict: native. A blocking callback can be a generic `impl Fn`. A stored callback is often a `Box<dyn FnOnce()>`.
