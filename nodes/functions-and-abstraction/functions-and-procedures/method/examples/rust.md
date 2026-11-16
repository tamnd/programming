```rust run id=method-rust
struct Rectangle {
    width: u32,
    height: u32,
}

impl Rectangle {
    fn area(&self) -> u32 {
        self.width * self.height
    }

    fn square(side: u32) -> Self {
        Self {
            width: side,
            height: side,
        }
    }
}

fn main() {
    let r = Rectangle {
        width: 3,
        height: 4,
    };
    println!("{}", r.area());
    println!("{}", Rectangle::square(5).area());
}
```

```output for=method-rust
12
25
```

- Verdict: native. `area` is a method because its first parameter is `&self`. `square` is an associated function with no `self`, and the program calls it with `Rectangle::square`.
