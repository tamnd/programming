```rust run id=body
use std::any::type_name;

fn type_of<T>(_: &T) -> &'static str {
    type_name::<T>()
}

fn mean(values: &[f64]) -> f64 {
    let total: f64 = values.iter().sum();
    total / values.len() as f64
}

fn main() {
    let mut v = Vec::new();
    v.push(5u8);
    let n = 7;
    let x = 2.5;
    let words: Vec<_> = "a bb ccc".split(' ').collect();
    println!("{}", type_of(&v));
    println!("{} {}", type_of(&n), type_of(&x));
    println!("{}", type_of(&words));
    println!("{}", mean(&[1.0, 2.0, 6.0]));
}
```

```output for=body
alloc::vec::Vec<u8>
i32 f64
alloc::vec::Vec<&str>
3
```

- Verdict: native. Inference works inside a function body. The later `push` of a `u8` fixes the type of `v`.
- An integer literal with no other constraint is an `i32`, and a floating-point literal is an `f64`.
- `Vec<_>` asks the compiler to infer only the element type. `collect` needs to know the collection type.
- Each signature, such as `mean`, gives the types of its parameters and its result. The compiler does not infer them.
