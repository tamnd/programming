```rust run id=mutability
use std::cell::Cell;

fn main() {
    let v = vec![1, 2, 3];
    let mut w = v.clone();
    w.push(4);
    println!("{v:?} {w:?}");

    let hits = Cell::new(0);
    let first = &hits;
    let second = &hits;
    first.set(first.get() + 1);
    second.set(second.get() + 1);
    println!("{}", hits.get());
}
```

```output for=mutability
[1, 2, 3] [1, 2, 3, 4]
2
```

- Verdict: native. A `let` binding is immutable unless it has `mut`. A shared reference `&T` stops changes to the value while the reference exists.
- Mutability belongs to the binding and the reference, not to the type. The same `Vec` is immutable through `v` and mutable through `w`.
- `Cell`, `RefCell` and the atomic types give interior mutability: a change through a shared reference. `UnsafeCell` is the only base for this.
- A mutable borrow of an immutable binding is the error E0596:

```rust fails expect=compile id=borrow-mut
fn main() {
    let v = vec![1, 2, 3];
    let r = &mut v;
    r.push(4);
    println!("{r:?}");
}
```

`rustc` stops with ``cannot borrow `v` as mutable, as it is not declared as mutable``. The fix is `let mut v`.
