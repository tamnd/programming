```rust run id=entry-or-default
use std::collections::BTreeMap;

fn main() {
    // entry().or_default() makes an empty Vec for a new key.
    let mut m: BTreeMap<&str, Vec<i32>> = BTreeMap::new();
    for (k, v) in [("yellow", 1), ("blue", 2), ("yellow", 3), ("blue", 4), ("red", 1)] {
        m.entry(k).or_default().push(v);
    }
    println!("{:?}", m);
    println!("{:?}", m.get("green").map(|v| v.len()).unwrap_or(0));

    // Remove one value. Remove the key when its Vec is empty.
    if let Some(v) = m.get_mut("red") {
        v.retain(|&x| x != 1);
        if v.is_empty() {
            m.remove("red");
        }
    }
    let pairs: usize = m.values().map(Vec::len).sum();
    println!("{} {}", m.len(), pairs);

    // Each key and each value, as flat pairs.
    let flat: Vec<(&str, i32)> = m.iter().flat_map(|(k, vs)| vs.iter().map(move |v| (*k, *v))).collect();
    println!("{:?}", flat);
}
```

```output
{"blue": [2, 4], "red": [1], "yellow": [1, 3]}
0
2 4
[("blue", 2), ("blue", 4), ("yellow", 1), ("yellow", 3)]
```

- Verdict: idiom. A HashMap or BTreeMap of Vec values, filled with entry(k).or_default().push(v). retain removes values, and the program removes a key when its Vec becomes empty.
