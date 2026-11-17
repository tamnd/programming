```rust run id=option-vec
use std::collections::HashMap;

fn main() {
    // Option and Vec take one type; HashMap takes two.
    let xs: Vec<Option<i32>> = vec![Some(1), None, Some(3)];
    let total: i32 = xs.iter().flatten().sum();

    let mut m: HashMap<&str, Vec<i32>> = HashMap::new();
    m.entry("a").or_default().push(total);

    println!("{xs:?} {total} {m:?}");
    println!("{}", std::any::type_name::<Vec<Option<i32>>>());
}
```

```output for=option-vec
[Some(1), None, Some(3)] 4 {"a": [4]}
alloc::vec::Vec<core::option::Option<i32>>
```

- Verdict: native. `Option` and `Vec` are generic types with one type parameter, and `HashMap` has two. `Vec<Option<i32>>` applies `Vec` to a type that is itself made by `Option`.

A type constructor without its type argument is not a type:

```rust fails expect=compile id=missing-argument
fn total(xs: &Vec) -> usize {
    xs.len()
}

fn main() {
    println!("{}", total(&vec![1, 2]));
}
```

The compiler reports error E0107: the struct `Vec` needs a type argument. Write `&Vec<i32>`, or make `total` generic.
