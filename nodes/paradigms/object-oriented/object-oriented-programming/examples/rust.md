```rust run id=trait-objects
trait Shape {
    fn area(&self) -> f64;
    fn describe(&self) -> String {
        format!("shape with area {:.2}", self.area())
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
        std::f64::consts::PI * self.r * self.r
    }
}

impl Shape for Square {
    fn area(&self) -> f64 {
        self.side * self.side
    }
    fn describe(&self) -> String {
        format!("square of side {}", self.side)
    }
}

fn main() {
    let shapes: Vec<Box<dyn Shape>> = vec![Box::new(Circle { r: 1.0 }), Box::new(Square { side: 2.0 })];
    for s in &shapes {
        println!("{}", s.describe());
    }
    let total: f64 = shapes.iter().map(|s| s.area()).sum();
    println!("{total:.2}");
}
```

```output for=trait-objects
shape with area 3.14
square of side 2
7.14
```

- Verdict: native. Rust has structs with methods, privacy at the module level, and traits with default methods. It has no inheritance of data between structs.
- A `Box<dyn Shape>` is a trait object: a pointer to the value and a pointer to a table of the methods of `Shape` for its type. Calls through it use dynamic dispatch. Generic functions with a `T: Shape` bound use static dispatch instead.
