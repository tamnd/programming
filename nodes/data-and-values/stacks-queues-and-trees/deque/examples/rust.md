```rust run id=deque
use std::collections::VecDeque;

fn main() {
    let mut d: VecDeque<i32> = VecDeque::from([2, 3]);
    d.push_front(1);
    d.push_back(4);
    println!("{d:?} front {:?} back {:?} d[2] {}", d.front(), d.back(), d[2]);

    let first = d.pop_front();
    let last = d.pop_back();
    println!("{first:?} {last:?} {d:?}");

    // After push_front, the items can sit in two parts of the ring buffer.
    let mut ring: VecDeque<i32> = VecDeque::with_capacity(4);
    ring.push_back(2);
    ring.push_back(3);
    ring.push_front(1);
    let (a, b) = ring.as_slices();
    println!("total {} items", a.len() + b.len());
    ring.make_contiguous();
    println!("contiguous {:?}", ring.as_slices().0);

    d.rotate_left(1);
    println!("rotate_left 1: {d:?}");
}
```

```output for=deque
[1, 2, 3, 4] front Some(1) back Some(4) d[2] 3
Some(1) Some(4) [2, 3]
total 3 items
contiguous [1, 2, 3]
rotate_left 1: [3, 2]
```

- Verdict: stdlib. `VecDeque` is a growable ring buffer with `push_front`, `push_back`, `pop_front`, `pop_back` and index access.
- The items can be in two parts of the buffer. `as_slices` returns both parts, and `make_contiguous` joins them.
