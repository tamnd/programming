```rust run id=dyn-any-downcast
use std::any::Any;

fn describe(value: &dyn Any) -> String {
    if let Some(n) = value.downcast_ref::<i32>() {
        format!("an i32, doubled {}", n * 2)
    } else if let Some(s) = value.downcast_ref::<String>() {
        format!("a String of {} bytes", s.len())
    } else {
        "something else".to_string()
    }
}

fn main() {
    let values: Vec<Box<dyn Any>> = vec![Box::new(21_i32), Box::new(String::from("hello")), Box::new(1.5_f64)];
    for v in &values {
        println!("{}", describe(v.as_ref()));
    }
    let boxed: Box<dyn Any> = Box::new(7_u8);
    match boxed.downcast::<i32>() {
        Ok(n) => println!("got {n}"),
        Err(_) => println!("downcast to i32 failed: the value is not an i32"),
    }
}
```

```output for=dyn-any-downcast
an i32, doubled 42
a String of 5 bytes
something else
downcast to i32 failed: the value is not an i32
```

- Verdict: stdlib. Rust is statically typed, and `std::any::Any` gives a checked form of dynamic typing for values whose type is not known where they are used. `downcast_ref` returns `None` and `Box::downcast` returns `Err` when the type does not match, so the program must handle the failure. A `u8` does not downcast to `i32`: there are no conversions, only an exact match of the type.
