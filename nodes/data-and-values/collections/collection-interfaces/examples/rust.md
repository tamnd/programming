```rust run id=to-the-interface
use std::collections::{BTreeMap, BTreeSet};

fn total<'a>(xs: impl IntoIterator<Item = &'a i32>) -> i32 {
    xs.into_iter().sum()
}

fn describe<'a, C>(c: &'a C) -> String
where
    &'a C: IntoIterator<Item = &'a i32>,
    <&'a C as IntoIterator>::IntoIter: ExactSizeIterator,
{
    let size = c.into_iter().len();
    format!("size {}, total {}", size, total(c))
}

fn main() {
    let list = vec![1, 2, 3];
    let set = BTreeSet::from([4, 5]);
    let prices = BTreeMap::from([("tea", 3), ("cake", 4)]);
    let values: Vec<i32> = prices.values().copied().collect(); // FromIterator

    println!("list: {}", describe(&list));
    println!("set: {}", describe(&set));
    println!("map values: {} (direct total {})", describe(&values), total(prices.values()));
}
```

```output for=to-the-interface
list: size 3, total 6
set: size 2, total 9
map values: size 2, total 7 (direct total 7)
```

- Verdict: stdlib. Rust has no `Collection` trait. `IntoIterator` lets a type be used in a `for` loop, `ExactSizeIterator` gives the length, and `FromIterator` lets `collect` build a collection.
- `describe` works with a reference to any collection whose iterator knows its length, such as `Vec` and `BTreeSet`. The compiler makes one copy of it for each type.
