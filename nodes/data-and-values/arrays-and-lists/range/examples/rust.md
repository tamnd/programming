```rust run id=range-types
fn main() {
    // start..end excludes end, and start..=end includes it.
    let r = 0..5;
    println!("{:?} {:?} {}", r.clone().collect::<Vec<_>>(), (1..=5).collect::<Vec<_>>(), r.len());

    // A range is a value: it has contains and is_empty, and an empty range has start >= end.
    println!("{} {} {} {}", (0..5).contains(&5), (0..=5).contains(&5), (5..2).is_empty(), (3..=3).is_empty());

    // step_by and rev change the order and the step.
    println!("{:?} {:?}", (0..10).step_by(3).collect::<Vec<_>>(), (0..5).rev().collect::<Vec<_>>());

    // Range<u8> cannot hold the range of all u8 values, because 256 does not fit in u8.
    println!("{}", (0u8..=255).count());

    // A range indexes a slice or a string.
    let a = [10, 20, 30, 40, 50];
    println!("{:?} {:?} {:?} {}", &a[1..3], &a[..2], &a[3..], &"hello"[1..4]);

    // An index past the end returns None with get, and panics with [].
    println!("{:?} {:?}", a.get(3..9), a.get(3..5));
}
```

```output
[0, 1, 2, 3, 4] [1, 2, 3, 4, 5] 5
false true true false
[0, 3, 6, 9] [4, 3, 2, 1, 0]
256
[20, 30] [10, 20] [40, 50] ell
None Some([40, 50])
```

- Verdict: native. 0..5 excludes 5 and 1..=5 includes it. A range is a value with len, contains and is_empty, and 5..2 is empty. step_by and rev change the step and the order. 0u8..=255 has 256 items, which Range<u8> cannot hold. A range indexes a slice or a string, and get returns None for a range past the end.
