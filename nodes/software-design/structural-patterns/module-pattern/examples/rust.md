```rust run id=counter-module
mod counter {
    use std::sync::atomic::{AtomicI64, Ordering};

    static PRIVATE_COUNTER: AtomicI64 = AtomicI64::new(0); // private by default

    fn change_by(val: i64) {
        PRIVATE_COUNTER.fetch_add(val, Ordering::SeqCst);
    }

    pub fn increment() {
        change_by(1);
    }

    pub fn decrement() {
        change_by(-1);
    }

    pub fn value() -> i64 {
        PRIVATE_COUNTER.load(Ordering::SeqCst)
    }
}

fn main() {
    println!("value: {}", counter::value());
    counter::increment();
    counter::increment();
    println!("value: {}", counter::value());
    counter::decrement();
    println!("value: {}", counter::value());
}
```

```output
value: 0
value: 2
value: 1
```

- Verdict: native. A `mod` block is a module, its items are private by default, and `pub` makes the API. `counter::change_by(1)` in `main` does not compile.
