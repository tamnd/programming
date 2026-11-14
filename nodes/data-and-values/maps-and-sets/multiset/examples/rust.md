```rust run id=entry-counts
use std::collections::BTreeMap;

fn count(s: &str) -> BTreeMap<char, usize> {
    let mut m = BTreeMap::new();
    for ch in s.chars() {
        *m.entry(ch).or_insert(0) += 1;
    }
    m
}

fn main() {
    let words = "the cat and the hat and the bat";
    let mut c: BTreeMap<&str, usize> = BTreeMap::new();
    for w in words.split_whitespace() {
        *c.entry(w).or_insert(0) += 1;
    }
    println!("{:?}", c);
    println!("{} {}", c["the"], c.get("dog").copied().unwrap_or(0));

    // The most common word: max by count, then by the smaller word on a tie.
    let top = c.iter().max_by(|a, b| a.1.cmp(b.1).then(b.0.cmp(a.0))).unwrap();
    println!("{:?}", top);

    // Two BTreeMaps compare equal when they have the same keys and counts.
    println!("{} {}", count("listen") == count("silent"), count("aab") == count("ab"));

    // Remove one copy; remove the key at zero.
    for w in ["and", "cat"] {
        if let Some(n) = c.get_mut(w) {
            *n -= 1;
            if *n == 0 {
                c.remove(w);
            }
        }
    }
    let total: usize = c.values().sum();
    println!("{:?} {} {} {}", c.get("and"), c.contains_key("cat"), c.len(), total);
}
```

```output
{"and": 2, "bat": 1, "cat": 1, "hat": 1, "the": 3}
3 0
("the", 3)
true false
Some(1) false 4 6
```

- Verdict: idiom. A HashMap<K, usize> or BTreeMap<K, usize> updated with *m.entry(k).or_insert(0) += 1. Two maps compare equal with ==. The standard library has no multiset type.
