```rust run id=dyn
use std::any::type_name;
use std::fmt::Display;
use std::mem::size_of;

fn show_static<T: Display>(value: T) -> String {
    format!("{} as {}", value, type_name::<T>())
}

fn show_dynamic(value: &dyn Display) -> String {
    format!("{}", value)
}

fn main() {
    println!("{}", show_static(7u8));
    println!("{}", show_static(1.5f32));
    let items: Vec<Box<dyn Display>> = vec![Box::new(7u8), Box::new("seven")];
    for item in &items {
        println!("{}", show_dynamic(item.as_ref()));
    }
    println!("{} {}", size_of::<&u8>(), size_of::<&dyn Display>());
}
```

```output for=dyn
7 as u8
1.5 as f32
7
seven
8 16
```

- Verdict: native. The compiler monomorphizes generic code, so each copy of `show_static` knows its `T`, and `type_name` reports it.
- `dyn Display` hides the concrete type. A `&dyn Display` is a pointer to the value and a pointer to a vtable. On this 64-bit machine, it is twice the size of a plain reference.
- A vector of `Box<dyn Display>` holds values of different types. A call through it goes through the vtable.
