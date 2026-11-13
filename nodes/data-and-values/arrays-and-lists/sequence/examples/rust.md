```rust run id=lazy-iterators
use std::cell::Cell;

fn main() {
    // An iterator is a sequence that gives one item per call to next.
    let v = vec![10, 20, 30];
    let mut it = v.iter();
    println!("{:?} {:?} {:?} {:?}", it.next(), it.next(), it.next(), it.next());

    // Adapters are lazy: map runs only when something consumes the iterator.
    let calls = Cell::new(0);
    let doubled = v.iter().map(|x| {
        calls.set(calls.get() + 1);
        x * 2
    });
    println!("calls before: {}", calls.get());
    let out: Vec<i32> = doubled.collect();
    println!("{:?} calls after: {}", out, calls.get());

    // 0.. is an infinite sequence. take makes it finite.
    let squares: Vec<u64> = (0..).map(|n: u64| n * n).filter(|n| n % 2 == 1).take(4).collect();
    println!("{:?}", squares);

    // A slice is an indexed sequence. Its iterator can also run backward.
    let s = &v[..];
    println!("{} {} {:?}", s.len(), s[1], s.iter().rev().collect::<Vec<_>>());

    // successors builds a sequence from a first item and a rule for the next one.
    let powers: Vec<u32> = std::iter::successors(Some(1u32), |&p| p.checked_mul(10)).collect();
    println!("{} {:?}", powers.len(), powers.last());
}
```

```output
Some(10) Some(20) Some(30) None
calls before: 0
[20, 40, 60] calls after: 3
[1, 9, 25, 49]
3 20 [30, 20, 10]
10 Some(1000000000)
```

- Verdict: native. next returns Some for each item and then None. map is lazy: the counter stays 0 until collect runs. The open range 0.. is infinite, and take makes it finite. A slice gives an iterator that can go in reverse. successors with checked_mul stops when the next value overflows.
