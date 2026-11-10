```rust run id=boxes
use std::mem::{size_of, size_of_val};

#[derive(Debug)]
enum List {
    Cons(i32, Box<List>),
    Nil,
}

fn main() {
    let buffer = [0u8; 1024];
    let boxed = Box::new([0u8; 1024]);
    println!("buffer in the frame: {} bytes", size_of_val(&buffer));
    println!("box in the frame is one pointer: {}", size_of_val(&boxed) == size_of::<usize>());
    println!("bytes behind the box: {}", size_of_val(&*boxed));

    let list = List::Cons(1, Box::new(List::Cons(2, Box::new(List::Nil))));
    println!("{list:?}");
}
```

```output
buffer in the frame: 1024 bytes
box in the frame is one pointer: true
bytes behind the box: 1024
Cons(1, Cons(2, Nil))
```

```rust fails expect=runtime id=overflow
fn descend(level: u64) -> u64 {
    if level == u64::MAX {
        return 0;
    }
    let buffer = std::hint::black_box([level; 64]);
    descend(level + 1) + buffer[0]
}

fn main() {
    println!("{}", descend(1));
}
```

- Verdict: native. A value lives in the frame of its owner by default. A `Box`, a `Vec` or another type that owns a heap block puts it on the heap.
- The array `buffer` takes 1024 bytes in the frame. The `Box` takes one pointer in the frame, and its 1024 bytes are on the heap.
- A recursive type needs a `Box`: without it, the size of `List` would have no limit.
- A stack overflow is not a panic. The runtime prints that the thread has overflowed its stack and aborts the process. A thread from `std::thread` gets 2 MiB of stack by default.
