```rust run id=display-and-debug
use std::fmt;

#[derive(Debug)]
struct Book {
    title: String,
    year: u16,
}

impl fmt::Display for Book {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{} ({})", self.title, self.year)
    }
}

fn main() {
    let b = Book { title: "Dune".to_string(), year: 1965 };
    println!("{b}");
    println!("{b:?}");
    println!("{b:#?}");
    let s: String = b.to_string();
    println!("{} {:?}", s.len(), vec![&b]);
}
```

```output for=display-and-debug
Dune (1965)
Book { title: "Dune", year: 1965 }
Book {
    title: "Dune",
    year: 1965,
}
11 [Book { title: "Dune", year: 1965 }]
```

- Verdict: stdlib. `Display` is the form for users, with `{}`, and `Debug` is the form for programmers, with `{:?}`. A type has neither until it derives or implements them.
- `Debug` can be derived and shows the type name and every field. `Display` cannot be derived; a type that implements it also gets `to_string` from the `ToString` trait.
- `{:#?}` pretty-prints the debug form on several lines. A vector prints the `Debug` form of its items.
