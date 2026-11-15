```rust run id=method-call-forms
struct Rectangle {
    width: u32,
    height: u32,
}

impl Rectangle {
    fn area(&self) -> u32 {
        self.width * self.height
    }

    fn scale(&mut self, factor: u32) {
        self.width *= factor;
        self.height *= factor;
    }
}

fn main() {
    let mut r = Rectangle { width: 3, height: 4 };
    r.scale(2); // the same as Rectangle::scale(&mut r, 2)
    let unbound: fn(&Rectangle) -> u32 = Rectangle::area;
    let bound = || r.area(); // a closure borrows r
    println!("{} {} {}", r.area(), Rectangle::area(&r), unbound(&r));
    println!("{}", bound());
    let mut list = vec![Rectangle { width: 5, height: 1 }, Rectangle { width: 2, height: 2 }];
    list.sort_by_key(Rectangle::area);
    println!("{}", list[0].width);
}
```

```output for=method-call-forms
48 48 48
48
2
```

- Verdict: native. Methods are functions in an `impl` block whose first parameter is `self`, `&self` or `&mut self`. The form of the receiver says whether the method takes the value, reads it or changes it.
- `r.scale(2)` is the method call syntax for `Rectangle::scale(&mut r, 2)`. Rust adds the `&` or `&mut` itself, by automatic referencing.
- `Rectangle::area` is a path to an ordinary function, so it can be passed to `sort_by_key`. A closure that borrows `r` is the bound form.
