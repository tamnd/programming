```rust run id=thunks
use std::cell::LazyCell;

fn load() -> i32 {
    println!("loading");
    21
}

fn main() {
    let plain = || load();
    println!("{}", plain() + plain());

    let memo = LazyCell::new(load);
    println!("{}", *memo + *memo);
}
```

```output for=thunks
loading
loading
42
loading
42
```

- Verdict: stdlib. A closure with no parameters is a plain thunk. `LazyCell` runs its function on the first dereference and stores the value. `LazyLock` is the form for a value that threads share, such as a `static`.
