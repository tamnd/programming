```rust run id=three-kinds
use std::collections::{BTreeMap, BTreeSet};

fn main() {
    let words = vec!["red", "blue", "red", "green"];
    let unique: BTreeSet<&str> = words.iter().copied().collect();
    let mut counts: BTreeMap<&str, usize> = BTreeMap::new();
    for w in &words {
        *counts.entry(w).or_insert(0) += 1;
    }

    println!("{} {} {}", words.len(), words[1], words.contains(&"red"));
    println!("{} {} {:?}", unique.len(), unique.contains("red"), unique);
    println!("{} {} {:?}", counts.len(), counts["red"], counts);
}
```

```output for=three-kinds
4 blue true
3 true {"blue", "green", "red"}
3 2 {"blue": 1, "green": 1, "red": 2}
```

- Verdict: stdlib. `Vec` is in the prelude, and `std::collections` has `VecDeque`, `LinkedList`, `HashMap`, `BTreeMap`, `HashSet`, `BTreeSet` and `BinaryHeap`.
- `collect` builds any collection from an iterator. The type of `unique` says which one.
- `BTreeSet` and `BTreeMap` keep their keys sorted. A `HashSet` or a `HashMap` gives its keys in an order that is not fixed.
