```rust run id=hashmap
use std::collections::{BTreeMap, HashMap};

fn main() {
    // insert returns the old value as an Option.
    let mut m: HashMap<&str, i32> = HashMap::from([("x", 1), ("y", 2)]);
    println!("{:?} {:?}", m.insert("x", 10), m.insert("z", 3));

    // get returns an Option, so a missing key is None, not an error.
    println!("{:?} {:?} {}", m.get("x"), m.get("w"), m.contains_key("y"));
    println!("{}", m.get("w").copied().unwrap_or(0));

    // m["w"] would panic, because indexing needs the key to be there.
    println!("{}", m["x"]);

    // remove returns the value that it took out.
    println!("{:?} {:?} {}", m.remove("y"), m.remove("y"), m.len());

    // The entry API inserts or updates with one lookup.
    *m.entry("x").or_insert(0) += 1;
    *m.entry("w").or_insert(0) += 1;

    // A HashMap has no order. Collect into a BTreeMap to print the pairs sorted.
    let sorted: BTreeMap<_, _> = m.iter().collect();
    println!("{sorted:?}");
}
```

```output
Some(1) None
Some(10) None true
0
10
Some(2) None 2
{"w": 1, "x": 11, "z": 3}
```

- Verdict: stdlib. insert returns the old value as an Option, get returns an Option, and indexing with a missing key panics. The entry API inserts or updates with one lookup. A HashMap has no order, so the example collects it into a BTreeMap to print it.
