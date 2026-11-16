```rust run id=ring
use std::collections::VecDeque;

struct RingBuffer<T> {
    items: VecDeque<T>,
    capacity: usize,
}

impl<T> RingBuffer<T> {
    fn new(capacity: usize) -> Self {
        RingBuffer {
            items: VecDeque::with_capacity(capacity),
            capacity,
        }
    }

    fn push(&mut self, item: T) {
        if self.items.len() == self.capacity {
            self.items.pop_front();
        }
        self.items.push_back(item);
    }

    fn pop(&mut self) -> Option<T> {
        self.items.pop_front()
    }
}

fn show(ring: &RingBuffer<i32>) -> String {
    let parts: Vec<String> = ring.items.iter().map(|n| n.to_string()).collect();
    parts.join(" ")
}

fn main() {
    let mut ring = RingBuffer::new(3);
    for reading in [10, 20, 30, 40, 50] {
        ring.push(reading);
        println!("after {reading}: {}", show(&ring));
    }

    if let Some(oldest) = ring.pop() {
        println!("read {oldest}, left: {}", show(&ring));
    }
}
```

```output for=ring
after 10: 10
after 20: 10 20
after 30: 10 20 30
after 40: 20 30 40
after 50: 30 40 50
read 30, left: 40 50
```

- Verdict: stdlib. `VecDeque` is a growable ring buffer. This wrapper calls `pop_front` before `push_back` when the length reaches the capacity, so the size stays fixed.
- `pop` returns `None` when the buffer is empty.
