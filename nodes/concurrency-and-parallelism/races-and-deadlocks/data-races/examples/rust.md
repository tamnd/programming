```rust run id=atomic
use std::sync::atomic::{AtomicU32, Ordering};
use std::thread;

fn main() {
    let count = AtomicU32::new(0);
    thread::scope(|s| {
        for _ in 0..2 {
            s.spawn(|| {
                for _ in 0..100_000 {
                    count.fetch_add(1, Ordering::Relaxed);
                }
            });
        }
    });
    println!("{}", count.into_inner());
}
```

```output for=atomic
200000
```

```rust fails expect=compile id=cell
use std::cell::Cell;
use std::thread;

fn main() {
    let count = Cell::new(0);
    thread::scope(|s| {
        s.spawn(|| count.set(count.get() + 1));
        s.spawn(|| count.set(count.get() + 1));
    });
    println!("{}", count.get());
}
```

- Verdict: native. The compiler checks `Send` and `Sync`, so safe code cannot share a `Cell` between threads. The second program fails with E0277, "`Cell<i32>` cannot be shared between threads safely".
- `AtomicU32` is `Sync`, so both threads can hold a shared reference to it. `fetch_add` with `Ordering::Relaxed` is atomic, and the end of `thread::scope` joins the threads before `into_inner`.
- `thread::scope` lets the threads borrow the local variable, because it joins every thread before it returns.
- The check does not cover `unsafe` code, and it does not prevent race conditions.
