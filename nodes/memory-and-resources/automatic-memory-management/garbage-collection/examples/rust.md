```rust run id=cycle
use std::cell::RefCell;
use std::rc::{Rc, Weak};

#[derive(Default)]
struct Item {
    strong_peer: RefCell<Option<Rc<Item>>>,
    weak_peer: RefCell<Weak<Item>>,
}

// Make a pair a -> b -> a and return a weak probe to a.
fn pair(back_link_is_weak: bool) -> Weak<Item> {
    let a = Rc::new(Item::default());
    let b = Rc::new(Item::default());
    *a.strong_peer.borrow_mut() = Some(Rc::clone(&b));
    if back_link_is_weak {
        *b.weak_peer.borrow_mut() = Rc::downgrade(&a);
    } else {
        *b.strong_peer.borrow_mut() = Some(Rc::clone(&a));
    }
    Rc::downgrade(&a)
}

fn main() {
    let leaked = pair(false);
    println!("strong cycle freed: {}", leaked.upgrade().is_none());
    println!("weak back link freed: {}", pair(true).upgrade().is_none());
    // Break the cycle by hand.
    if let Some(a) = leaked.upgrade() {
        a.strong_peer.borrow_mut().take();
    }
    println!("freed after manual break: {}", leaked.upgrade().is_none());
}
```

```output
strong cycle freed: false
weak back link freed: true
freed after manual break: true
```

- Verdict: stdlib. Rust has no collector. Each value has one owner, and the value is dropped when its owner goes out of scope.
- `Rc` adds shared ownership with a reference count. A cycle of `Rc` values keeps every count above zero, so the pair made by `pair(false)` leaks until the cycle is broken by hand.
- `Rc::downgrade` makes a `Weak` pointer, which does not count as an owner. `upgrade` returns `None` once the value is gone.
- `Arc` is the thread-safe form of `Rc`, with atomic counts. It has the same cycle problem.
