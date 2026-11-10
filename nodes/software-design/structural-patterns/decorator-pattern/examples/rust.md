```rust run id=repository
use std::collections::HashMap;

trait Repository {
    fn get(&mut self, key: &str) -> String;
    fn name(&self) -> String;
}

struct Store;

impl Repository for Store {
    fn get(&mut self, key: &str) -> String {
        println!("store: load {key}");
        format!("value-{key}")
    }

    fn name(&self) -> String {
        "store".to_string()
    }
}

struct Logged<R: Repository> {
    inner: R,
}

impl<R: Repository> Repository for Logged<R> {
    fn get(&mut self, key: &str) -> String {
        println!("log: get {key}");
        self.inner.get(key)
    }

    fn name(&self) -> String {
        self.inner.name()
    }
}

struct Cached<R: Repository> {
    inner: R,
    cache: HashMap<String, String>,
}

impl<R: Repository> Cached<R> {
    fn new(inner: R) -> Self {
        Cached { inner, cache: HashMap::new() }
    }
}

impl<R: Repository> Repository for Cached<R> {
    fn get(&mut self, key: &str) -> String {
        if let Some(value) = self.cache.get(key) {
            return value.clone();
        }
        let value = self.inner.get(key);
        self.cache.insert(key.to_string(), value.clone());
        value
    }

    fn name(&self) -> String {
        self.inner.name()
    }
}

fn main() {
    let mut repo = Logged { inner: Cached::new(Store) };
    for key in ["a", "a", "b"] {
        println!("{}", repo.get(key));
    }
    println!("name: {}", repo.name());
}
```

```output
log: get a
store: load a
value-a
log: get a
value-a
log: get b
store: load b
value-b
name: store
```

- Verdict: idiom. Each decorator is a generic struct that owns its inner value. The type of `repo` is `Logged<Cached<Store>>`, so the compiler knows the whole chain and can inline the calls.
- `get` takes `&mut self` because the cache changes. A cache behind `&self` needs `RefCell` or a `Mutex`.
- To choose the chain at run time, the decorators can hold a `Box<dyn Repository>` in place of the type parameter.
- The standard library uses this shape. `BufReader<R>` wraps any `R: Read`, and the iterator adapters such as `map` and `filter` wrap an iterator and are iterators.
