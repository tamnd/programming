```rust run id=hash-agrees-with-eq
use std::collections::HashSet;
use std::hash::{Hash, Hasher};

// A tag whose equality ignores case.
#[derive(Debug)]
struct Tag(String);

impl PartialEq for Tag {
    fn eq(&self, other: &Self) -> bool {
        self.0.eq_ignore_ascii_case(&other.0)
    }
}

impl Eq for Tag {}

// The hash must ignore case too, or equal tags land in different buckets.
impl Hash for Tag {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.0.to_ascii_lowercase().hash(state);
    }
}

fn main() {
    let mut tags = HashSet::new();
    tags.insert(Tag("Rust".to_string()));
    tags.insert(Tag("RUST".to_string()));
    tags.insert(Tag("rust".to_string()));
    println!("{} {}", tags.len(), tags.contains(&Tag("rUsT".to_string())));

    let x = f64::NAN;
    println!("{} {}", x == x, x.partial_cmp(&x).is_none());
}
```

```output for=hash-agrees-with-eq
1 true
false true
```

- Verdict: stdlib. `Eq` and `Hash` are traits, and `HashSet` and `HashMap` need both. A hand-written `PartialEq` that ignores case needs a `Hash` that ignores case too.
- The compiler does not check that the two traits agree; a `Hash` that reads the original string would compile and give a set with 3 tags.
- `f64` implements `PartialEq` and not `Eq`, because `NaN` is not equal to itself, so `HashSet<f64>` does not compile.
