```rust run id=list
use std::mem::size_of;

enum List {
    Cons(i32, Box<List>),
    Nil,
}

use List::{Cons, Nil};

fn total(xs: &List) -> i32 {
    match xs {
        Cons(head, tail) => head + total(tail),
        Nil => 0,
    }
}

fn length(xs: &List) -> usize {
    match xs {
        Cons(_, tail) => 1 + length(tail),
        Nil => 0,
    }
}

fn show(xs: &List) -> String {
    let mut parts = Vec::new();
    let mut node = xs;
    while let Cons(head, tail) = node {
        parts.push(head.to_string());
        node = &**tail;
    }
    parts.join(" -> ")
}

fn main() {
    let xs = Cons(1, Box::new(Cons(2, Box::new(Cons(3, Box::new(Nil))))));
    println!("sum: {}", total(&xs));
    println!("length: {}", length(&xs));
    println!("list: {}", show(&xs));
    let same = size_of::<Box<List>>() == size_of::<usize>();
    println!("a box is one pointer: {}", if same { "yes" } else { "no" });
}
```

```output for=list
sum: 6
length: 3
list: 1 -> 2 -> 3
a box is one pointer: yes
```

- Verdict: native. The `Cons` variant holds a `Box<List>`, a pointer to a `List` on the heap. Without the box, the compiler reports that the type has infinite size.
- A `Box` has the size of one pointer, so the compiler knows the size of `List`.
- `Rc<List>` in place of `Box<List>` lets two lists share a tail.
