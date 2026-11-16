```rust run id=dyn-any
use std::any::Any;

fn describe(v: &dyn Any) -> String {
    if let Some(n) = v.downcast_ref::<i32>() {
        format!("i32 {}", n + 1)
    } else if let Some(s) = v.downcast_ref::<String>() {
        format!("String {}", s.to_uppercase())
    } else if v.is::<&str>() {
        "a &str".to_string()
    } else {
        "unknown".to_string()
    }
}

fn main() {
    let values: Vec<Box<dyn Any>> = vec![
        Box::new(42_i32),
        Box::new(String::from("hi")),
        Box::new("lit"),
        Box::new(3.5_f64),
    ];
    for v in &values {
        println!("{}", describe(v.as_ref()));
    }

    // A downcast to the wrong type gives the box back as an error.
    let b: Box<dyn Any> = Box::new(7_u8);
    match b.downcast::<i32>() {
        Ok(n) => println!("i32 {n}"),
        Err(b) => println!("not an i32, but u8 is {}", b.is::<u8>()),
    }
}
```

```output for=dyn-any
i32 43
String HI
a &str
unknown
not an i32, but u8 is true
```

- Verdict: stdlib. Rust has no top type and no root class. The trait object `dyn Any` stores the type of the value, so `downcast_ref` and `is` can test it when the program runs. Code must test each type it knows, and `3.5` matches none of them here. `v.as_ref()` passes the value in the box, not the box itself.
