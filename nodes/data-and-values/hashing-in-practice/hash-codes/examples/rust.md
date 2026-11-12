```rust run id=derive-hash-and-random-state
use std::collections::hash_map::RandomState;
use std::collections::HashMap;
use std::hash::{BuildHasher, Hash, Hasher};

#[derive(Debug, PartialEq, Eq, Hash)] // derived together, so they agree
struct Point {
    x: i32,
    y: i32,
}

struct Account {
    id: String,
    visits: u32, // not part of equality, so not part of the hash
}

impl PartialEq for Account {
    fn eq(&self, other: &Self) -> bool { self.id == other.id }
}
impl Eq for Account {}
impl Hash for Account {
    fn hash<H: Hasher>(&self, state: &mut H) { self.id.hash(state) }
}

fn main() {
    let mut points = HashMap::new();
    points.insert(Point { x: 1, y: 2 }, "a");
    println!("struct key: {:?}", points.get(&Point { x: 1, y: 2 }));

    let mut ages: HashMap<String, u32> = HashMap::new();
    ages.insert("Alan".to_string(), 1912);
    println!("lookup by &str: {:?}", ages.get("Alan")); // String: Borrow<str>

    let (a, b) = (Account { id: "ada".into(), visits: 1 }, Account { id: "ada".into(), visits: 7 });
    let s = RandomState::new();
    println!("same id: {}, same hash: {}, visits {} and {}", a == b, s.hash_one(&a) == s.hash_one(&b), a.visits, b.visits);

    // Each RandomState has its own random keys for SipHash
    let (s1, s2) = (RandomState::new(), RandomState::new());
    println!("same state: {}, two states: {}", s1.hash_one("hello") == s1.hash_one("hello"), s1.hash_one("hello") == s2.hash_one("hello"));
}
```

```output for=derive-hash-and-random-state
struct key: Some("a")
lookup by &str: Some(1912)
same id: true, same hash: true, visits 1 and 7
same state: true, two states: false
```

```rust fails expect=error:E0277 id=key-without-hash
use std::collections::HashSet;

#[derive(PartialEq, Eq)]
struct Point {
    x: i32,
    y: i32,
}

fn main() {
    let mut seen = HashSet::new();
    seen.insert(Point { x: 1, y: 2 });
    println!("{}", seen.len());
}
```

- Verdict: stdlib. The `Hash` trait and `HashMap` are in the standard library, and `#[derive(PartialEq, Eq, Hash)]` makes equality and hash agree. A manual implementation, such as `Account`, must hash only the fields that `eq` compares. `HashMap` uses SipHash 1-3 with random keys from `RandomState`, so two states hash the same string differently. The `Borrow` trait lets a map with `String` keys look up a `&str`. The fails block uses a type without `Hash` as a key, and the compiler reports E0277.
