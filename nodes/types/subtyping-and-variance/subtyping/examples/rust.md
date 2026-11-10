```rust run id=lifetimes
trait Shape {
    fn area(&self) -> f64;
}

struct Square(f64);

impl Shape for Square {
    fn area(&self) -> f64 {
        self.0 * self.0
    }
}

struct Circle(f64);

impl Shape for Circle {
    fn area(&self) -> f64 {
        std::f64::consts::PI * self.0 * self.0
    }
}

fn longest<'a>(x: &'a str, y: &'a str) -> &'a str {
    if x.len() >= y.len() { x } else { y }
}

fn main() {
    let owned = String::from("hello there");
    let fixed: &'static str = "hi";
    println!("{}", longest(&owned, fixed));

    let shapes: Vec<Box<dyn Shape>> = vec![Box::new(Square(2.0)), Box::new(Circle(1.0))];
    let total: f64 = shapes.iter().map(|s| s.area()).sum();
    println!("{total:.2}");
}
```

```output for=lifetimes
hello there
7.14
```

- Verdict: native. Rust has subtyping only between lifetimes. `&'static str` is a subtype of `&'a str`, so `fixed` is accepted where `longest` expects `&'a str`.
- A trait is not a supertype of the types that implement it. `Box<Square>` becomes `Box<dyn Shape>` by an unsizing coercion, and the trait object calls `area` through a vtable.
- `&'a mut T` is invariant in `T`, which is the rule for mutable containers.
