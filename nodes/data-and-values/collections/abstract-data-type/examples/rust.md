```rust run id=stack-adt
mod stacks {
    pub trait Stack {
        fn push(&mut self, v: i32);
        fn pop(&mut self) -> Option<i32>;
        fn is_empty(&self) -> bool;
    }

    #[derive(Default)]
    pub struct ArrayStack {
        items: Vec<i32>, // private to the module
    }

    impl Stack for ArrayStack {
        fn push(&mut self, v: i32) { self.items.push(v); }
        fn pop(&mut self) -> Option<i32> { self.items.pop() }
        fn is_empty(&self) -> bool { self.items.is_empty() }
    }

    struct Cell {
        v: i32,
        rest: Option<Box<Cell>>,
    }

    #[derive(Default)]
    pub struct LinkedStack {
        top: Option<Box<Cell>>,
    }

    impl Stack for LinkedStack {
        fn push(&mut self, v: i32) {
            let rest = self.top.take();
            self.top = Some(Box::new(Cell { v, rest }));
        }
        fn pop(&mut self) -> Option<i32> {
            self.top.take().map(|c| {
                let Cell { v, rest } = *c;
                self.top = rest;
                v
            })
        }
        fn is_empty(&self) -> bool { self.top.is_none() }
    }
}

use stacks::{ArrayStack, LinkedStack, Stack};

fn drain(s: &mut dyn Stack) -> String {
    for v in 1..=3 {
        s.push(v);
    }
    let mut out = Vec::new();
    while let Some(v) = s.pop() {
        out.push(v.to_string());
    }
    out.join(" ")
}

fn main() {
    println!("ArrayStack: {}", drain(&mut ArrayStack::default()));
    println!("LinkedStack: {}", drain(&mut LinkedStack::default()));
}
```

```output for=stack-adt
ArrayStack: 3 2 1
LinkedStack: 3 2 1
```

- Verdict: native. A trait names the operations. The fields `items` and `top` have no `pub`, so only code inside the module `stacks` can use them.
- Code outside the module cannot write `ArrayStack { items: vec![] }`, so it uses `ArrayStack::default()` to make a value.
