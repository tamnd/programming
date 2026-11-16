```rust run id=pq
use std::cmp::Reverse;
use std::collections::BinaryHeap;

fn main() {
    let input = [
        (3, "update docs"),
        (1, "fix outage"),
        (2, "review patch"),
        (4, "plan sprint"),
    ];
    let mut tasks = BinaryHeap::new();
    for (priority, name) in input {
        tasks.push(Reverse((priority, name)));
    }

    if let Some(Reverse((_, name))) = tasks.peek() {
        println!("peek: {name}");
    }
    while let Some(Reverse((priority, name))) = tasks.pop() {
        println!("{priority} {name}");
    }

    let mut max_heap = BinaryHeap::from([3, 1, 2, 4]);
    let mut order = Vec::new();
    while let Some(n) = max_heap.pop() {
        order.push(n.to_string());
    }
    println!("largest first: {}", order.join(" "));
}
```

```output for=pq
peek: fix outage
1 fix outage
2 review patch
3 update docs
4 plan sprint
largest first: 4 3 2 1
```

- Verdict: stdlib. `BinaryHeap` gives the largest item first. `Reverse` turns it into a min-heap.
- `peek` returns an `Option` with a reference, and `pop` returns an `Option` with the item.
