```rust run id=iterators
use std::sync::LazyLock;

static CONFIG: LazyLock<String> = LazyLock::new(|| {
    println!("reading config");
    String::from("debug")
});

fn main() {
    let squares = (1..).map(|n| n * n).filter(|n| n % 2 == 1);
    let first: Vec<u64> = squares.take(3).collect();
    println!("{first:?}");

    println!("program started");
    println!("{} {}", *CONFIG, *CONFIG);
}
```

```output for=iterators
[1, 9, 25]
program started
reading config
debug debug
```

- Verdict: stdlib. Rust evaluates eagerly, but iterator adapters such as `map` and `filter` are lazy. The Rust book states that iterators "have no effect until you call methods that consume the iterator".
- `LazyLock` and `LazyCell` compute a value on first access. `OnceLock` stores a value that code sets one time.
- An adapter that nothing consumes does nothing, and the compiler warns about it, because the adapters are `#[must_use]`.
