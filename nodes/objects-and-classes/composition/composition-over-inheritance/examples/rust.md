```rust run id=generic-part
use std::collections::{BTreeSet, HashSet};

// The part that Counting needs from a set.
trait Set {
    fn insert(&mut self, x: String) -> bool;
    fn len(&self) -> usize;

    // A default method: every Set gets it without writing it.
    fn describe(&self) -> String {
        format!("{} items", self.len())
    }
}

impl Set for HashSet<String> {
    fn insert(&mut self, x: String) -> bool {
        HashSet::insert(self, x)
    }
    fn len(&self) -> usize {
        HashSet::len(self)
    }
}

impl Set for BTreeSet<String> {
    fn insert(&mut self, x: String) -> bool {
        BTreeSet::insert(self, x)
    }
    fn len(&self) -> usize {
        BTreeSet::len(self)
    }
}

// Composition: Counting has-a set of any type that implements Set.
struct Counting<S: Set> {
    inner: S,
    added: usize,
}

impl<S: Set> Counting<S> {
    fn new(inner: S) -> Self {
        Counting { inner, added: 0 }
    }

    fn add(&mut self, x: &str) {
        self.added += 1;
        self.inner.insert(x.to_string());
    }

    fn add_all(&mut self, xs: &[&str]) {
        for x in xs {
            self.add(x);
        }
    }
}

fn main() {
    let mut s = Counting::new(HashSet::new());
    s.add("a");
    s.add_all(&["b", "c", "a"]);
    println!("{} {}", s.added, s.inner.describe());

    let mut t = Counting::new(BTreeSet::new());
    t.add_all(&["c", "a", "b"]);
    println!("{} {:?}", t.added, t.inner);
}
```

```output for=generic-part
4 3 items
3 {"a", "b", "c"}
```

- Verdict: native. Rust has no inheritance between structs, so a struct reuses code by holding other values in its fields. `Counting<S>` holds any type that implements the trait `Set`, and the compiler makes a separate copy of the code for each type, so the forwarding costs nothing at run time. A default method in a trait, such as `describe`, shares code between all the types that implement the trait. A field of type `Box<dyn Set>` would let the part change at run time, with a virtual call.
