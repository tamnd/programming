```rust run id=traits
use std::fmt;
use std::mem::size_of;

trait Shape {
    fn area(&self) -> f64;

    fn describe(&self) -> String {
        format!("area {:.2}", self.area())
    }
}

struct Square {
    side: f64,
}

struct Circle {
    radius: f64,
}

impl Shape for Square {
    fn area(&self) -> f64 {
        self.side * self.side
    }
}

impl Shape for Circle {
    fn area(&self) -> f64 {
        std::f64::consts::PI * self.radius * self.radius
    }

    fn describe(&self) -> String {
        format!("circle of radius {}", self.radius)
    }
}

impl fmt::Display for Square {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "square of side {}", self.side)
    }
}

fn total_static<T: Shape>(items: &[T]) -> f64 {
    items.iter().map(|s| s.area()).sum()
}

fn total_dynamic(items: &[Box<dyn Shape>]) -> f64 {
    items.iter().map(|s| s.area()).sum()
}

fn main() {
    let squares = [Square { side: 1.0 }, Square { side: 2.0 }];
    println!("{:.2}", total_static(&squares));
    println!("{}", squares[1]);

    let mixed: Vec<Box<dyn Shape>> = vec![Box::new(Square { side: 2.0 }), Box::new(Circle { radius: 1.0 })];
    for s in &mixed {
        println!("{}", s.describe());
    }
    println!("{:.2}", total_dynamic(&mixed));

    println!("{} {}", size_of::<&Square>(), size_of::<&dyn Shape>());
}
```

```output
5.00
square of side 2
area 4.00
circle of radius 1
7.14
8 16
```

- Verdict: native. A trait declares methods, and `impl Shape for Square` implements them for one type. The `impl` block is separate from the type, so a crate can implement its own trait for a type from another crate, such as `f64`.
- `total_static` is generic. The compiler makes one copy of it for each type `T` that the program uses, and each copy calls `area` directly. This is static dispatch.
- `total_dynamic` takes trait objects. A call to `area` goes through the vtable of each object. This is dynamic dispatch.
- On a 64-bit target, `&Square` is one pointer of 8 bytes. `&dyn Shape` is 16 bytes: a pointer to the data and a pointer to the vtable.
