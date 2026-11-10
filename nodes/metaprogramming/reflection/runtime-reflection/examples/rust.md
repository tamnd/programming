```rust run id=any
use std::any::{Any, type_name};

#[derive(Debug)]
struct Point {
    x: i32,
    y: i32,
}

fn describe(value: &dyn Any) -> String {
    if let Some(p) = value.downcast_ref::<Point>() {
        format!("a Point with x = {} and y = {}", p.x, p.y)
    } else if let Some(n) = value.downcast_ref::<i32>() {
        format!("an i32 equal to {n}")
    } else {
        String::from("a value of an unknown type")
    }
}

fn name_of<T>(_: &T) -> &'static str {
    type_name::<T>()
}

fn main() {
    let p = Point { x: 3, y: 4 };
    println!("{}", describe(&p));
    println!("{}", describe(&7_i32));
    println!("{}", describe(&"text"));
    println!("{}", name_of(&1.5_f64));
    println!("{p:?}");
}
```

```output for=any
a Point with x = 3 and y = 4
an i32 equal to 7
a value of an unknown type
f64
Point { x: 3, y: 4 }
```

`dyn Any` can only answer one question: is this value of the concrete type `T`? It cannot list the fields of `Point` or find a method by name. `#[derive(Debug)]` prints the fields, but the derive macro writes that code at compile time.

- Verdict: absent. Rust has no run-time reflection of fields or methods. The module `std::any` gives `TypeId`, `type_name` and downcasting of `dyn Any` to a concrete type.
- `&dyn Any` cannot test whether a type implements a trait.
- Code that needs the structure of a type, such as a serializer, uses a derive macro that generates the code at compile time.
