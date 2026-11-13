```rust run id=vec-capacity
fn main() {
    // Vec<T> keeps the items in a heap buffer with a length and a capacity.
    let mut v: Vec<i32> = Vec::new();
    let mut caps = vec![v.capacity()];
    for i in 0..100 {
        v.push(i);
        if *caps.last().unwrap() != v.capacity() {
            caps.push(v.capacity());
        }
    }
    println!("{} {:?}", v.len(), caps);

    // with_capacity and reserve get the memory before the pushes.
    let mut w = Vec::with_capacity(10);
    let start = w.as_ptr();
    for i in 0..10 {
        w.push(i);
    }
    println!("{} {}", w.capacity() >= 10, start == w.as_ptr());

    // A Vec does not shrink by itself. shrink_to_fit gives the memory back.
    v.truncate(3);
    let before = v.capacity();
    v.shrink_to_fit();
    println!("{} {} {}", v.len(), before >= 100, v.capacity() >= 3);

    // insert(0, x) moves every other item one place to the right.
    v.insert(0, -1);
    println!("{:?}", v);
}
```

```output
100 [0, 4, 8, 16, 32, 64, 128]
true true
3 true true
[-1, 0, 1, 2]
```

- Verdict: stdlib. Vec starts with capacity 0 and then has 4, 8, 16, 32, 64 and 128 slots for i32 items. with_capacity keeps the same buffer for 10 pushes. truncate keeps the large buffer, and shrink_to_fit makes it smaller. insert(0, x) moves the items.
