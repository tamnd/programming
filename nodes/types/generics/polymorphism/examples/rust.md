```rust run id=static-dynamic
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
        std::f64::consts::PI * self.0 * self.0
    }
}

// Static dispatch: the compiler makes a copy of the function for each type.
fn show_static<T: Shape>(s: &T) -> String {
    format!("{:.2}", s.area())
}

// Dynamic dispatch: the call goes through the vtable of the trait object.
fn show_dyn(s: &dyn Shape) -> String {
    format!("{:.2}", s.area())
}

fn main() {
    println!("{} {}", show_static(&Square(2.0)), show_static(&Circle(1.0)));

    let shapes: Vec<Box<dyn Shape>> = vec![Box::new(Square(2.0)), Box::new(Circle(1.0))];
    let areas: Vec<String> = shapes.iter().map(|s| show_dyn(s.as_ref())).collect();
    println!("{}", areas.join(" "));
}
```

```output for=static-dynamic
4.00 3.14
4.00 3.14
```

- Verdict: native. `show_static` has a type parameter with the bound `Shape`, so the compiler makes one copy for `Square` and one for `Circle`. `show_dyn` takes a `&dyn Shape`, and the call finds `area` when the program runs. A `Vec<Box<dyn Shape>>` can hold values of both types, but a `Vec<T>` holds one type. Rust has no overloading of functions.
