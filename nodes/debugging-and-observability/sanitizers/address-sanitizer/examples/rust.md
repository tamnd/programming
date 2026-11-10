```rust fails expect=panic id=index
fn main() {
    let scores = vec![3, 1, 4];
    let mut total = 0;
    for i in 0..=scores.len() {
        total += scores[i]; // index 3 is out of bounds
    }
    println!("{total}");
}
```

Safe Rust checks each index. The read of `scores[3]` stops the program with a panic before it touches memory outside the vector. This is the check that ASan adds to C.

```rust fails expect=compile id=dangling
fn main() {
    let view: &str;
    {
        let name = String::from("alice");
        view = &name;
    }
    println!("{view}");
}
```

```output for=dangling stream=stderr
error[E0597]: `name` does not live long enough
 --> main.rs:5:16
  |
4 |         let name = String::from("alice");
  |             ---- binding `name` declared here
5 |         view = &name;
  |                ^^^^^ borrowed value does not live long enough
6 |     }
  |     - `name` dropped here while still borrowed
7 |     println!("{view}");
  |                ---- borrow later used here

error: aborting due to 1 previous error

For more information about this error, try `rustc --explain E0597`.
```

The borrow checker rejects the program at compile time. `view` would outlive `name`, so the program cannot read freed memory, and there is nothing for ASan to find.

```rust run id=checked
fn main() {
    let scores = vec![3, 1, 4];
    match scores.get(3) {
        Some(value) => println!("{value}"),
        None => println!("index 3 is out of range"),
    }
    let total: i32 = scores.iter().sum();
    println!("{total}");
}
```

```output for=checked
index 3 is out of range
8
```

- Verdict: library. `rustc -Zsanitizer=address` builds a program with ASan. The flag is unstable, so it needs a nightly compiler, and the Rust documentation recommends a rebuild of the standard library with `-Zbuild-std`.
- Safe Rust has bounds checks and the borrow checker, so ASan is for `unsafe` code and for C or C++ code that Rust calls.
- `get` returns an `Option`, so the caller must handle an index that is out of range.
