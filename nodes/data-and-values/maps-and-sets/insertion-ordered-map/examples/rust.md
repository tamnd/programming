```rust run id=vec-and-index
use std::collections::HashMap;

// The standard library has no insertion-ordered map. This one keeps the entries
// in a Vec and an index from each key to its position, like the indexmap crate.
struct IndexMap {
    entries: Vec<(String, i32)>,
    index: HashMap<String, usize>,
}

impl IndexMap {
    fn new() -> Self {
        IndexMap { entries: Vec::new(), index: HashMap::new() }
    }

    fn insert(&mut self, k: &str, v: i32) {
        match self.index.get(k) {
            Some(&i) => self.entries[i].1 = v,
            None => {
                self.index.insert(k.to_string(), self.entries.len());
                self.entries.push((k.to_string(), v));
            }
        }
    }

    fn get(&self, k: &str) -> Option<i32> {
        self.index.get(k).map(|&i| self.entries[i].1)
    }

    // Fast remove: move the last entry into the hole. The order changes.
    fn swap_remove(&mut self, k: &str) {
        if let Some(i) = self.index.remove(k) {
            self.entries.swap_remove(i);
            if i < self.entries.len() {
                let moved = self.entries[i].0.clone();
                self.index.insert(moved, i);
            }
        }
    }

    // Slow remove: shift the later entries down. The order stays, but this is linear.
    fn shift_remove(&mut self, k: &str) {
        if let Some(i) = self.index.remove(k) {
            self.entries.remove(i);
            for (j, (key, _)) in self.entries.iter().enumerate().skip(i) {
                self.index.insert(key.clone(), j);
            }
        }
    }

    fn keys(&self) -> Vec<&str> {
        self.entries.iter().map(|(k, _)| k.as_str()).collect()
    }
}

fn main() {
    let mut m = IndexMap::new();
    for (k, v) in [("a", 1), ("b", 2), ("c", 3), ("d", 4), ("e", 5)] {
        m.insert(k, v);
    }
    m.insert("a", 10);
    println!("{:?} {:?}", m.keys(), m.get("a"));

    let mut s = IndexMap { entries: m.entries.clone(), index: m.index.clone() };
    m.swap_remove("b");
    println!("{:?} {:?}", m.keys(), m.get("e"));
    s.shift_remove("b");
    println!("{:?} {:?}", s.keys(), s.get("e"));

    // A HashMap iterates in an arbitrary order, so the output sorts the keys.
    let h: HashMap<&str, i32> = [("pear", 4), ("apple", 7), ("fig", 2)].into_iter().collect();
    let mut ks: Vec<_> = h.keys().copied().collect();
    ks.sort();
    println!("{:?}", ks);
}
```

```output
["a", "b", "c", "d", "e"] Some(10)
["a", "e", "c", "d"] Some(5)
["a", "c", "d", "e"] Some(5)
["apple", "fig", "pear"]
```

- Verdict: idiom. The standard library has no insertion-ordered map, and HashMap iterates in an arbitrary order. The example keeps a Vec of entries and a HashMap from key to position, like the indexmap crate. swap_remove is constant time but changes the order, and shift_remove keeps the order but is linear.
