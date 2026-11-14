```rust run id=hash-trait
use std::collections::HashMap;
use std::hash::{BuildHasher, RandomState};

// derive(Hash, PartialEq, Eq) makes hash and equality agree, field by field.
#[derive(Hash, PartialEq, Eq, Debug)]
struct Point {
    x: i32,
    y: i32,
}

fn main() {
    let mut grid = HashMap::new();
    grid.insert(Point { x: 1, y: 2 }, "found");
    println!("{:?}", grid.get(&Point { x: 1, y: 2 }));

    // Each RandomState has its own random keys, so one value hashes differently in two maps.
    let s1 = RandomState::new();
    let s2 = RandomState::new();
    println!("{} {}", s1.hash_one("key") == s1.hash_one("key"), s1.hash_one("key") == s2.hash_one("key"));

    // with_capacity makes room for 1000 entries with no resize.
    let mut m: HashMap<i32, i32> = HashMap::with_capacity(1000);
    let cap = m.capacity();
    for i in 0..1000 {
        m.insert(i, i * i);
    }
    println!("{} {} {}", cap >= 1000, m.capacity() == cap, m[&999]);

    // f64 does not implement Eq or Hash, because NaN != NaN.
    // HashMap<f64, i32> does not compile; a common fix is to use the bits as the key.
    let mut by_bits: HashMap<u64, &str> = HashMap::new();
    by_bits.insert(1.5f64.to_bits(), "one and a half");
    println!("{:?}", by_bits.get(&1.5f64.to_bits()));

    // Hash is defined for tuples, strings and Vec, so they are keys too.
    let mut pairs: HashMap<(String, Vec<u8>), i32> = HashMap::new();
    pairs.insert(("a".to_string(), vec![1, 2]), 7);
    println!("{}", pairs[&("a".to_string(), vec![1, 2])]);
}
```

```output
Some("found")
true false
true true 998001
Some("one and a half")
7
```

- Verdict: stdlib. derive(Hash, PartialEq, Eq) makes a struct a key. Each RandomState has random keys, so one value hashes differently in two maps. with_capacity makes room for n entries. f64 is not Eq or Hash, so a program uses the bits of a float as the key.
