```rust run id=vec-and-cons-list
use std::collections::LinkedList;

// A list as in Lisp or Haskell: empty, or a first item and the rest.
#[derive(Debug)]
enum List {
    Nil,
    Cons(i32, Box<List>),
}

use List::{Cons, Nil};

fn sum(list: &List) -> i32 {
    match list {
        Nil => 0,
        Cons(first, rest) => first + sum(rest),
    }
}

fn main() {
    // Vec is the usual list in Rust.
    let mut v = vec![3, 1, 2];
    v.push(4);
    v.insert(0, 9);
    println!("{:?} {} {:?}", v, v.len(), v.iter().position(|&x| x == 2));

    // A cons list needs Box, because a type cannot contain itself directly.
    let c = Cons(1, Box::new(Cons(2, Box::new(Cons(3, Box::new(Nil))))));
    println!("{:?} {}", c, sum(&c));

    // std::collections::LinkedList is a doubly linked list.
    let mut l: LinkedList<i32> = LinkedList::new();
    l.push_back(2);
    l.push_front(1);
    println!("{:?} {:?}", l, l.front());
}
```

```output
[9, 3, 1, 2, 4] 5 Some(3)
Cons(1, Cons(2, Cons(3, Nil))) 6
[1, 2] Some(1)
```

- Verdict: stdlib. Vec is the usual list. A cons list is an enum with a Box for the rest, and a recursive function sums it. LinkedList is a doubly linked list with push_front and pop_front.
