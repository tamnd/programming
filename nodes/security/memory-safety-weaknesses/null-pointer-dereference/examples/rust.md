```rust fails expect=panic id=unwrap
fn find_quota(name: &str) -> Option<i32> {
    match name {
        "alice" => Some(10),
        "bob" => Some(20),
        _ => None,
    }
}

fn main() {
    for name in ["alice", "nobody"] {
        let quota = find_quota(name).unwrap();
        println!("{name}: quota {quota}");
    }
}
```

`unwrap` on `None` panics, and the message says that `Option::unwrap()` was called on a `None` value. The panic is defined behavior and stops the thread, but it is still a crash. Each `unwrap` and `expect` is a place to review.

```rust run id=match
fn find_quota(name: &str) -> Option<i32> {
    match name {
        "alice" => Some(10),
        "bob" => Some(20),
        _ => None,
    }
}

fn main() {
    for name in ["alice", "nobody"] {
        match find_quota(name) {
            Some(quota) => println!("{name}: quota {quota}"),
            None => println!("{name}: no such user"),
        }
    }
}
```

```output for=match
alice: quota 10
nobody: no such user
```

- Verdict: native. Safe Rust has no null references. `Option<T>` is a different type from `T`, so the compiler refuses a use of the value until the code handles `None`.
- `unwrap` and `expect` turn a `None` into a panic. Use `match`, `if let`, `?` or `unwrap_or` on values that come from input.
- Raw pointers in `unsafe` code can be null. Use `ptr::NonNull` or check with `is_null` before a dereference.
