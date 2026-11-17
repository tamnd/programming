```rust run id=mono-and-dyn
use std::mem::size_of;

trait Shape {
    fn area(&self) -> f64;
}

struct Square(f64);
struct Circle(f64);

impl Shape for Square {
    fn area(&self) -> f64 {
        self.0 * self.0
    }
}

impl Shape for Circle {
    fn area(&self) -> f64 {
        3.0 * self.0 * self.0
    }
}

// A copy of this function is made for each type T.
fn area_of<T: Shape>(s: &T) -> f64 {
    s.area()
}

// One copy: the call goes through the vtable.
fn total(shapes: &[&dyn Shape]) -> f64 {
    shapes.iter().map(|s| s.area()).sum()
}

fn main() {
    println!("{} {}", area_of(&Square(2.0)), area_of(&Circle(1.0)));
    println!("{}", total(&[&Square(2.0), &Circle(1.0)]));
    println!("{} {}", size_of::<&Square>(), size_of::<&dyn Shape>());
}
```

```output for=mono-and-dyn
4 3
7
8 16
```

- Verdict: native. `area_of` is monomorphized: the compiler makes one version for `Square` and one for `Circle`. `total` takes trait objects, so one copy of the code works for both. A `&dyn Shape` is twice the size of a `&Square`, because it holds a pointer to the vtable too.
