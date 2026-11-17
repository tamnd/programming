```rust run id=five-needs
use std::collections::{BTreeMap, HashMap, HashSet, VecDeque};

fn main() {
    let words: Vec<&str> = "the cat saw the dog and the cat ran".split(' ').collect();

    let seen: HashSet<&str> = words.iter().copied().collect();
    let mut counts: HashMap<&str, usize> = HashMap::new();
    for &w in &words {
        *counts.entry(w).or_insert(0) += 1;
    }
    let mut line: VecDeque<&str> = words[..3].iter().copied().collect();
    let first = line.pop_front().unwrap();
    let second = line.pop_front().unwrap();
    let sorted: BTreeMap<&str, usize> = counts.iter().map(|(k, v)| (*k, *v)).collect();

    println!("list: {} words, third is {}", words.len(), words[2]);
    println!("set: {} distinct", seen.len());
    println!("map: the={} cat={}", counts["the"], counts["cat"]);
    println!("queue: {}, then {}", first, second);
    let keys: Vec<&str> = sorted.keys().copied().collect();
    println!("sorted: {}", keys.join(" "));
}
```

```output for=five-needs
list: 9 words, third is saw
set: 6 distinct
map: the=3 cat=2
queue: the, then cat
sorted: and cat dog ran saw the
```

- `Vec`, `HashSet`, `HashMap`, `VecDeque` and `BTreeMap` are in `std::collections` or the prelude. The `BTreeMap` keeps its keys sorted.
