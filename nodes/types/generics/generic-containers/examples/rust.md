```rust run id=typed-containers
use std::collections::HashMap;

#[derive(Debug, PartialEq, Eq, Hash, Clone, Copy)]
enum Color {
    Red,
    Blue,
}

fn main() {
    let mut v: Vec<i32> = Vec::new();
    v.push(3);
    v.extend([1, 2]);
    v.sort();
    println!("{:?}", v);

    // The key type must implement Eq and Hash.
    let mut counts: HashMap<Color, usize> = HashMap::new();
    for c in [Color::Red, Color::Blue, Color::Red] {
        *counts.entry(c).or_insert(0) += 1;
    }
    println!("{} {}", counts[&Color::Red], counts[&Color::Blue]);
}
```

```output for=typed-containers
[1, 2, 3]
2 1
```

- Verdict: stdlib. `Vec<T>` and `HashMap<K, V>` are in the standard library. `Color` derives `Eq` and `Hash`, so it can be a key of a `HashMap`.
