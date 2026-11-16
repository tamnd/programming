```rust run id=overloading-rust
trait Show {
    fn show(&self) -> String;
}

impl Show for i32 {
    fn show(&self) -> String {
        format!("int {}", self)
    }
}

impl Show for &str {
    fn show(&self) -> String {
        format!("string {}", self)
    }
}

impl Show for (i32, i32) {
    fn show(&self) -> String {
        format!("two ints {} {}", self.0, self.1)
    }
}

fn main() {
    let n: i32 = 42;
    let pair: (i32, i32) = (3, 4);
    println!("{}", n.show());
    println!("{}", "hi".show());
    println!("{}", pair.show());
}
```

```output for=overloading-rust
int 42
string hi
two ints 3 4
```

- Verdict: idiom. Rust has no overloading. The trait `Show` has one method, and each argument type, including the tuple `(i32, i32)`, implements it.
