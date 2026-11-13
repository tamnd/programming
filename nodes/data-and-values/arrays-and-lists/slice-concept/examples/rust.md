```rust run id=borrowed-slices
fn sum(items: &[i32]) -> i32 {
    items.iter().sum()
}

fn main() {
    // &v[a..b] borrows a part of a Vec or an array as a slice: a pointer and a length.
    let v = vec![10, 20, 30, 40, 50];
    let part = &v[1..3];
    println!("{:?} {} {} {}", part, part.len(), sum(part), sum(&v[3..]));

    // A mutable slice changes the items of the Vec.
    let mut w = v.clone();
    let tail = &mut w[3..];
    tail[0] = 99;
    tail.reverse();
    println!("{:?}", w);

    // split_at_mut gives two mutable slices that do not overlap.
    let (left, right) = w.split_at_mut(2);
    left[0] += right[0];
    println!("{:?} {:?}", left, right);

    // get returns None for a range out of bounds. &v[3..9] would panic.
    println!("{:?} {:?}", v.get(3..9), v.get(3..5));

    // A string slice must start and end on a character boundary.
    let s = "héllo";
    println!("{} {} {}", &s[0..1], &s[1..3], s.is_char_boundary(2));

    // to_vec copies a slice into a new Vec.
    let mut owned = part.to_vec();
    owned.push(1);
    println!("{:?} {:?}", owned, part);
}
```

```output
[20, 30] 2 50 90
[10, 20, 30, 50, 99]
[40, 20] [30, 50, 99]
None Some([40, 50])
h é false
[20, 30, 1] [20, 30]
```

- Verdict: native. &v[1..3] borrows a part of a Vec, and one function takes slices of any length. A mutable slice changes the Vec, and split_at_mut gives two parts that do not overlap. get returns None for a range out of bounds. A string slice must end on a character boundary, and to_vec copies.
