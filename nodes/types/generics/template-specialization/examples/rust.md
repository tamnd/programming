```rust fails expect=compile id=overlap
trait Describe {
    fn describe(&self) -> String;
}

// A blanket impl for every type that can be printed.
impl<T: std::fmt::Debug> Describe for T {
    fn describe(&self) -> String {
        format!("some value {:?}", self)
    }
}

// A special impl for i32 overlaps the blanket impl.
impl Describe for i32 {
    fn describe(&self) -> String {
        format!("the int {}", self)
    }
}

fn main() {
    println!("{}", 7.describe());
}
```

```rust run id=separate-impls
trait Describe {
    fn describe(&self) -> String;
}

// Stable Rust: one impl for each type, with no overlap.
impl Describe for f64 {
    fn describe(&self) -> String {
        format!("some value {}", self)
    }
}

impl Describe for i32 {
    fn describe(&self) -> String {
        format!("the int {}", self)
    }
}

fn main() {
    println!("{}", 2.5.describe());
    println!("{}", 7.describe());
}
```

```output for=separate-impls
some value 2.5
the int 7
```

- Verdict: absent. The first program does not compile, because the blanket impl for every `Debug` type and the impl for `i32` overlap. Specialization would let the more specific impl win, but it is an unstable feature. Stable Rust gives each type its own impl.
