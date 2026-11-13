```rust run id=iterator-chains
use std::collections::{BTreeMap, BTreeSet};

fn main() {
    let words = ["apple", "fig", "banana", "kiwi", "cherry"];

    // An iterator chain: a source, a filter, a map and collect.
    let lengths: Vec<usize> = words.iter().filter(|w| w.len() > 3).map(|w| w.len()).collect();
    println!("{:?}", lengths);

    // collect builds the collection that the type names.
    let firsts: BTreeSet<char> = words.iter().map(|w| w.chars().next().unwrap()).collect();
    let with_i: BTreeMap<&str, usize> =
        words.iter().filter(|w| w.contains('i')).map(|w| (*w, w.len())).collect();
    println!("{:?} {:?}", firsts, with_i);

    // flat_map does the work of two for clauses.
    let pairs: Vec<String> = (1..=2)
        .flat_map(|x| ["a", "b"].iter().map(move |y| format!("{x}{y}")))
        .collect();
    println!("{:?}", pairs);

    // Adapters are lazy: nothing runs until collect, and take stops the work early.
    let mut calls = 0;
    let first_two: Vec<String> = words
        .iter()
        .map(|w| {
            calls += 1;
            w.to_uppercase()
        })
        .take(2)
        .collect();
    println!("{:?} {}", first_two, calls);
}
```

```output
[5, 6, 4, 6]
{'a', 'b', 'c', 'f', 'k'} {"fig": 3, "kiwi": 4}
["1a", "1b", "2a", "2b"]
["APPLE", "FIG"] 2
```

- Verdict: stdlib. Rust has no comprehension syntax, but an iterator chain has the same parts. collect builds the type that the program names, here a Vec, a BTreeSet and a BTreeMap. flat_map does the work of two for clauses. The adapters are lazy, so the closure in map runs only twice because take(2) needs only two items.
