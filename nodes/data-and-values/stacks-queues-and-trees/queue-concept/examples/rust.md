```rust run id=queue
use std::collections::VecDeque;

fn main() {
    let mut queue: VecDeque<&str> = VecDeque::new();
    for item in ["a", "b", "c"] {
        queue.push_back(item);
    }
    let front = queue.front();
    println!("{queue:?} front {front:?}");

    let first = queue.pop_front();
    println!("{first:?} next {:?} size {}", queue.front(), queue.len());
    queue.pop_front();
    queue.pop_front();
    println!("pop_front on empty: {:?}", queue.pop_front());
}
```

```output for=queue
["a", "b", "c"] front Some("a")
Some("a") next Some("b") size 2
pop_front on empty: None
```

- Verdict: stdlib. `VecDeque` is the queue. `push_back` adds at the back, `pop_front` removes the front, and `front` peeks.
- `pop_front` returns an `Option`, so an empty queue gives `None`.
