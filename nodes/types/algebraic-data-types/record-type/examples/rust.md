```rust run id=record
#[derive(Debug, Clone, PartialEq)]
struct Person {
    name: String,
    age: u32,
}

fn yn(b: bool) -> &'static str {
    if b { "yes" } else { "no" }
}

fn main() {
    let ada = Person {
        name: String::from("Ada"),
        age: 36,
    };
    println!("name: {} age: {}", ada.name, ada.age);

    let older = Person {
        age: ada.age + 1,
        ..ada.clone()
    };
    println!("copy: {:?}", older);
    println!("original: {:?}", ada);

    let other = Person {
        name: String::from("Ada"),
        age: 36,
    };
    println!("same fields equal: {}", yn(ada == other));
}
```

```output for=record
name: Ada age: 36
copy: Person { name: "Ada", age: 37 }
original: Person { name: "Ada", age: 36 }
same fields equal: yes
```

- Verdict: native. A `struct` with named fields is a record. `derive` generates `Debug`, `Clone` and `PartialEq`.
- `..ada.clone()` takes the remaining fields from a clone. With `..ada`, the `String` field moves into the new value, and `ada` can no longer be used as a whole.
- A binding is immutable unless it is declared with `mut`.
