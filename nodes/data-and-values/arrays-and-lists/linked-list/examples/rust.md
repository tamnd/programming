```rust run id=box-stack
use std::collections::LinkedList;

// A singly linked stack. Each node owns the rest of the list through a Box.
struct Node {
    value: i32,
    next: Option<Box<Node>>,
}

struct Stack {
    head: Option<Box<Node>>,
}

impl Stack {
    fn new() -> Self {
        Stack { head: None }
    }

    fn push(&mut self, value: i32) {
        let old = self.head.take();
        self.head = Some(Box::new(Node { value, next: old }));
    }

    fn pop(&mut self) -> Option<i32> {
        self.head.take().map(|node| {
            self.head = node.next;
            node.value
        })
    }

    // Reverse in place by moving each Box to a new head. No node is copied.
    fn reverse(&mut self) {
        let mut prev = None;
        let mut cur = self.head.take();
        while let Some(mut node) = cur {
            cur = node.next.take();
            node.next = prev;
            prev = Some(node);
        }
        self.head = prev;
    }

    fn to_vec(&self) -> Vec<i32> {
        let mut out = Vec::new();
        let mut cur = &self.head;
        while let Some(node) = cur {
            out.push(node.value);
            cur = &node.next;
        }
        out
    }
}

fn main() {
    let mut s = Stack::new();
    for v in 1..=4 {
        s.push(v);
    }
    println!("{:?}", s.to_vec());
    s.reverse();
    println!("{:?}", s.to_vec());
    println!("{:?} {:?}", s.pop(), s.to_vec());

    // std::collections::LinkedList can join two lists in constant time.
    let mut a: LinkedList<i32> = LinkedList::from([1, 2]);
    let mut b: LinkedList<i32> = LinkedList::from([3, 4]);
    a.append(&mut b);
    println!("{:?} {}", a, b.len());
}
```

```output
[4, 3, 2, 1]
[1, 2, 3, 4]
Some(1) [2, 3, 4]
[1, 2, 3, 4] 0
```

- Verdict: stdlib. A hand-written singly linked stack uses Option<Box<Node>>, and take moves each Box, so push, pop and reverse copy no node. std::collections::LinkedList::append moves all nodes of b into a and leaves b empty.
