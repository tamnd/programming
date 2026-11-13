```rust run id=fixed-size-arrays
fn sum(items: &[i32]) -> i32 {
    items.iter().sum()
}

fn main() {
    // [T; N]: the length is part of the type. [0; 5] repeats a value.
    let mut a = [0; 5];
    a[0] = 7;
    a[4] = 9;
    println!("{:?} {}", a, a.len());

    // An array of Copy items is Copy, so assignment copies all the items.
    let mut b = a;
    b[1] = 1;
    println!("{:?} {:?} {}", a, b, a == b);

    // An array coerces to a slice, so slice methods and functions work on it.
    println!("{} {:?}", sum(&a), a.iter().max());

    // get returns None for an index out of range. a[i] would panic.
    let i = 5;
    println!("{:?} {:?}", a.get(1), a.get(i));

    // The items are stored side by side with no header.
    println!("{} {}", std::mem::size_of::<[i32; 5]>(), std::mem::size_of::<[u8; 3]>());
}
```

```output
[7, 0, 0, 0, 9] 5
[7, 0, 0, 0, 9] [7, 1, 0, 0, 9] false
16 Some(9)
Some(0) None
20 3
```

- Verdict: native. [0; 5] repeats a value, and the length is part of the type. An array of Copy items is Copy, so b is a separate array. An array coerces to a slice for functions such as sum. get returns None for index 5, and the size of [i32; 5] is 20 bytes with no header.
