```rust run id=trait-objects
use std::fmt::Display;
use std::rc::Rc;

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

fn show(items: &[&dyn Display]) {
    for item in items {
        print!("{item} ");
    }
    println!();
}

fn main() {
    let shapes: Vec<Box<dyn Shape>> = vec![Box::new(Square(2.0)), Box::new(Circle(1.0))];
    for s in &shapes {
        print!("{} ", s.area()); // each object answers with its own method
    }
    println!();

    show(&[&42, &"hi", &2.5]);
    println!(
        "{}",
        std::mem::size_of::<&dyn Shape>() / std::mem::size_of::<usize>()
    );

    let a = Rc::new(String::from("shared"));
    let b = Rc::clone(&a); // a second owner of the same object
    println!("{} {}", Rc::ptr_eq(&a, &b), Rc::strong_count(&a));
}
```

```output for=trait-objects
4 3 
42 hi 2.5 
2
true 2
```

- Verdict: idiom. Rust has no objects in the sense of Smalltalk or Java: structs and enums hold data, and `impl` blocks give them methods. A trait object, such as `Box<dyn Shape>` or `&dyn Display`, is the closest form: a pointer to the data plus a pointer to a table of the methods of its type, which is why `&dyn Shape` is two words wide. Each object in the vector answers `area` with the method of its own type.
- Values have one owner and move by default, so sharing one object needs a reference or a shared pointer such as `Rc`. `Rc::ptr_eq` compares identity, and the count shows two owners.
