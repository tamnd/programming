```rust run id=counts
use std::rc::Rc;
use std::sync::Arc;
use std::thread;

fn main() {
    let order = Rc::new(String::from("order 42"));
    let shared = Rc::clone(&order);
    let probe = Rc::downgrade(&order);
    println!("strong {}, weak {}", Rc::strong_count(&order), Rc::weak_count(&order));
    drop(shared);
    drop(order);
    println!("value alive: {}", probe.upgrade().is_some());

    let total = Arc::new(vec![1, 2, 3]);
    let workers: Vec<_> = (0..3)
        .map(|_| {
            let total = Arc::clone(&total);
            thread::spawn(move || total.iter().sum::<i32>())
        })
        .collect();
    let sums: Vec<i32> = workers.into_iter().map(|w| w.join().unwrap()).collect();
    println!("sums {:?}, strong count after join {}", sums, Arc::strong_count(&total));
}
```

```output
strong 2, weak 1
value alive: false
sums [6, 6, 6], strong count after join 1
```

- Verdict: stdlib. `Rc` and `Arc` in the standard library count the owners of one heap allocation. `Rc::clone` adds an owner, and the drop of an owner removes one.
- `Rc::downgrade` makes a `Weak` pointer. It adds to the weak count, not to the strong count, and `upgrade` returns `None` after the value is dropped.
- `Rc` uses plain counts and is not `Send`, so the compiler rejects it in `thread::spawn`. `Arc` uses atomic counts. Each thread drops its clone when it ends, so the count is 1 again after `join`.
- Most Rust values have one owner and need no count; see [[rust/ownership]].
