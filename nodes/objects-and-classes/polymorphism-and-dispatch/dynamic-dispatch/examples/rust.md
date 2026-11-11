```rust run id=shapes
use std::f64::consts::PI;
use std::mem::size_of;

trait Shape {
    fn area(&self) -> f64;
    fn name(&self) -> &'static str {
        "shape"
    }
    fn describe(&self) -> String {
        format!("{} with area {:.2}", self.name(), self.area())
    }
}

struct Circle {
    r: f64,
}
struct Square {
    side: f64,
}

impl Shape for Circle {
    fn area(&self) -> f64 {
        PI * self.r * self.r
    }
    fn name(&self) -> &'static str {
        "circle"
    }
}

impl Shape for Square {
    fn area(&self) -> f64 {
        self.side * self.side
    }
}

// Static dispatch: the compiler makes one copy of the function for each T.
fn total_static<T: Shape>(xs: &[T]) -> f64 {
    xs.iter().map(|s| s.area()).sum()
}

// Dynamic dispatch: one copy; each call goes through the vtable of the object.
fn total_dyn(xs: &[Box<dyn Shape>]) -> f64 {
    xs.iter().map(|s| s.area()).sum()
}

fn main() {
    let shapes: Vec<Box<dyn Shape>> = vec![Box::new(Circle { r: 1.0 }), Box::new(Square { side: 2.0 })];
    for s in &shapes {
        println!("{}", s.describe());
    }
    let squares = [Square { side: 1.0 }, Square { side: 2.0 }];
    println!("{:.2} {:.2}", total_dyn(&shapes), total_static(&squares));
    println!("{} {}", size_of::<&Square>(), size_of::<&dyn Shape>());
}
```

```output for=shapes
circle with area 3.14
shape with area 4.00
7.14 5.00
8 16
```

```rust fails expect=error:E0038 id=generic-method
trait Show {
    fn show<T: std::fmt::Debug>(&self, extra: T) -> String; // a generic method
}

struct A;

impl Show for A {
    fn show<T: std::fmt::Debug>(&self, extra: T) -> String {
        format!("A {extra:?}")
    }
}

fn main() {
    let v: Box<dyn Show> = Box::new(A);
    println!("{}", v.show(1));
}
```

- Verdict: native. A trait object, `&dyn Shape` or `Box<dyn Shape>`, is a wide pointer of two words: a pointer to the data and a pointer to the vtable of the concrete type for that trait. The last line shows the sizes on a 64-bit machine. A generic function with a trait bound, such as `total_static`, uses static dispatch instead, and the program chooses between the two at each use.
- The failing block puts a generic method in a trait and then uses the trait as `dyn Show`. A vtable has one slot for each method, but a generic method has a copy for each type `T`, so it cannot have a slot, and the compiler rejects the trait object with E0038. The Rust Book calls these rules dyn compatibility.
