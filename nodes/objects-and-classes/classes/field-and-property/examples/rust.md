```rust run id=fields-and-accessors
mod shapes {
    pub struct Size {
        pub width: u32, // public fields
        pub height: u32,
    }

    pub struct Rectangle {
        width: u32, // private to the module
        height: u32,
    }

    impl Rectangle {
        pub fn new(width: u32, height: u32) -> Self {
            Rectangle { width, height }
        }

        pub fn width(&self) -> u32 {
            self.width // a getter has the name of the field
        }

        pub fn set_width(&mut self, width: u32) {
            self.width = width;
        }

        pub fn area(&self) -> u32 {
            self.width * self.height
        }
    }
}

use shapes::{Rectangle, Size};

fn main() {
    let mut s = Size { width: 3, height: 4 };
    s.width = 5;
    println!("{} {}", s.width, s.height);

    let mut r = Rectangle::new(3, 4);
    println!("{} {}", r.width(), r.area());
    r.set_width(5);
    println!("{}", r.area());
}
```

```output for=fields-and-accessors
5 4
3 12
20
```

- Verdict: idiom. Rust has struct fields and no properties. A field is private to its module unless it is marked `pub`, so code outside `shapes` cannot write `r.width` and must call the methods.
- A getter has the name of the field, `width()`, and a setter `set_width`. A method with `&self` reads, and one with `&mut self` changes the struct.
- A type such as `u32` can carry part of the check: a negative width cannot exist, so `set_width` has no check to make.
