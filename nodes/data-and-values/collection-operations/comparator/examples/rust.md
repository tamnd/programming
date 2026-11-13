```rust run id=ordering-and-total-cmp
use std::cmp::{Ordering, Reverse};

fn main() {
    // cmp returns an Ordering: Less, Equal or Greater.
    println!("{:?} {:?} {:?}", 1.cmp(&2), 2.cmp(&2), "b".cmp("a"));

    let mut people = vec![("cy", 31), ("ann", 31), ("bob", 25)];

    // sort_by takes a compare function; then_with adds a second key.
    people.sort_by(|a, b| a.1.cmp(&b.1).then_with(|| a.0.cmp(b.0)));
    println!("{:?}", people);

    // sort_by_key takes a key; Reverse turns the order of one key around.
    people.sort_by_key(|p| (Reverse(p.1), p.0));
    println!("{:?}", people);

    // f64 is not Ord, because NaN has no place in a total order. total_cmp gives one.
    let mut fs = vec![2.0, f64::NAN, 1.0, -0.0, 0.0];
    fs.sort_by(|a, b| a.total_cmp(b));
    println!("{:?}", fs);

    // partial_cmp returns None for NaN.
    println!("{:?} {:?}", 1.0f64.partial_cmp(&2.0), f64::NAN.partial_cmp(&1.0));

    // reverse() on an Ordering swaps Less and Greater.
    let mut nums = vec![3, 1, 2];
    nums.sort_by(|a, b| a.cmp(b).reverse());
    println!("{:?} {:?}", nums, Ordering::Less.reverse());
}
```

```output
Less Equal Greater
[("bob", 25), ("ann", 31), ("cy", 31)]
[("ann", 31), ("cy", 31), ("bob", 25)]
[-0.0, 0.0, 1.0, 2.0, NaN]
Some(Less) None
[3, 2, 1] Greater
```

- Verdict: native. `cmp` returns an `Ordering`. `sort_by` with `then_with` sorts by age and then by name, and `sort_by_key` with `Reverse` sorts by age in descending order. `f64` is not `Ord`, so the program uses `total_cmp`, which puts `-0.0` before `0.0` and `NaN` last. `partial_cmp` returns `None` for `NaN`. `reverse` on an `Ordering` swaps `Less` and `Greater`.
