```rust run id=derive-traits
use std::collections::HashSet;

#[derive(Debug, Clone, PartialEq, Eq, Hash, PartialOrd, Ord)]
struct Version {
    major: u32,
    minor: u32,
}

fn main() {
    let a = Version { major: 1, minor: 4 };
    let b = Version { minor: 10, ..a.clone() };
    println!("{:?}", a);
    println!("{} {}", a == Version { major: 1, minor: 4 }, a < b);

    let seen: HashSet<Version> = [a.clone(), b.clone(), a.clone()].into_iter().collect();
    println!("{}", seen.len());

    let mut all = vec![b, a];
    all.sort();
    println!("{:?}", all);
}
```

```output for=derive-traits
Version { major: 1, minor: 4 }
true true
2
[Version { major: 1, minor: 4 }, Version { major: 1, minor: 10 }]
```

- Verdict: stdlib. `#[derive]` writes each listed trait from the fields: `Debug` for the debug form, `PartialEq` and `Eq` for `==`, `Hash` for hash sets and maps, and `PartialOrd` and `Ord` for `<` and sorting.
- The derived order compares the fields in their order of declaration.
- The struct update syntax `..a.clone()` takes the other fields from `a`, so it makes a copy with changes.
