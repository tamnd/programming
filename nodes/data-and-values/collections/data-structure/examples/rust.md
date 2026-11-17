```rust run id=two-layouts
struct Node {
    value: i32,
    next: Option<Box<Node>>,
}

fn get(mut node: &Node, mut i: usize) -> (i32, usize) {
    let mut steps = 0;
    while i > 0 {
        node = node.next.as_deref().expect("index out of range");
        i -= 1;
        steps += 1;
    }
    (node.value, steps)
}

fn main() {
    let array = [10, 20, 30, 40, 50];
    let mut head: Option<Box<Node>> = None;
    for &v in array.iter().rev() {
        head = Some(Box::new(Node { value: v, next: head }));
    }

    println!("array[3] = {} (0 steps)", array[3]);
    let (value, steps) = get(head.as_deref().unwrap(), 3);
    println!("linked[3] = {} ({} steps)", value, steps);

    head = Some(Box::new(Node { value: 5, next: head }));
    let mut out = Vec::new();
    let mut n = head.as_deref();
    while let Some(m) = n {
        out.push(m.value.to_string());
        n = m.next.as_deref();
    }
    println!("after push_front: {}", out.join(" "));
}
```

```output for=two-layouts
array[3] = 40 (0 steps)
linked[3] = 40 (3 steps)
after push_front: 5 10 20 30 40 50
```

- Verdict: stdlib. `std::collections` has `Vec`, `VecDeque`, `LinkedList`, `HashMap`, `BTreeMap` and `BinaryHeap`.
- A node owns the next node through `Option<Box<Node>>`. `None` ends the list.
