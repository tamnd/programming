```rust run id=fixpoint
use std::collections::BTreeSet;

fn main() {
    let parent = [("zeus", "ares"), ("ares", "harmonia"), ("harmonia", "semele"), ("semele", "dionysus")];

    let mut ancestor: BTreeSet<(&str, &str)> = parent.iter().copied().collect();
    loop {
        let derived: Vec<_> = parent
            .iter()
            .flat_map(|&(x, z)| ancestor.iter().filter(move |&&(z2, _)| z2 == z).map(move |&(_, y)| (x, y)))
            .collect();
        let before = ancestor.len();
        ancestor.extend(derived);
        if ancestor.len() == before {
            break;
        }
    }
    println!("{}", ancestor.len());
    let roots: Vec<_> = ancestor.iter().filter(|&&(_, y)| y == "harmonia").map(|&(x, _)| x).collect();
    println!("{roots:?}");
}
```

```output for=fixpoint
10
["ares", "zeus"]
```

- Verdict: absent. Rust has no logic engine in the language. The trait solver of the compiler is itself a logic program over impls, and crates such as `datafrog` and `ascent` give Datalog to Rust programs.
