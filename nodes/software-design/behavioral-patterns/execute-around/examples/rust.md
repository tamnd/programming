```rust run id=around
fn with_connection<T>(name: &str, block: impl FnOnce(&str) -> T) -> T {
    struct Closer<'a>(&'a str);
    impl Drop for Closer<'_> {
        fn drop(&mut self) {
            println!("close {}", self.0); // runs even if block panics
        }
    }
    println!("open {}", name);
    let _closer = Closer(name);
    block(name) // the caller's work
}

fn main() {
    let result = with_connection("db", |_conn| {
        let rows = ["a", "b", "c"];
        println!("query: {} rows", rows.len());
        rows.len()
    });
    println!("result: {}", result);
}
```

```output for=around
open db
query: 3 rows
close db
result: 3
```

- Verdict: idiom. A closure is the block, and a value with `Drop` does the cleanup.
