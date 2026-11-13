```rust run id=usize-and-get
fn main() {
    let a = [10, 20, 30, 40, 50];
    // Indices are usize and go from 0 to len - 1.
    println!("{} {} {}", a[0], a[a.len() - 1], a.len());

    // get returns None for an index out of bounds. a[i] would panic.
    println!("{:?} {:?} {:?} {:?}", a.get(4), a.get(5), a.first(), a.last());

    // An index cannot be negative. A signed value must be converted, and the conversion can fail.
    let i: i64 = -1;
    println!("{:?} {:?}", usize::try_from(i), usize::try_from(3i64).map(|k| a[k]));

    // checked_sub avoids an underflow when the code computes the last index of an empty slice.
    let empty: [i32; 0] = [];
    println!("{:?} {:?}", empty.len().checked_sub(1), a.len().checked_sub(1));

    // Count from the end with len - k, or with iter().rev().
    let k = 2;
    println!("{} {:?}", a[a.len() - k], a.iter().rev().nth(k - 1));

    // A str index is a byte range, and it must end on a character boundary.
    let s = "héllo";
    println!("{} {:?} {:?}", s.len(), s.get(0..2), s.get(0..3));
}
```

```output
10 50 5
Some(50) None Some(10) Some(50)
Err(TryFromIntError(NegOverflow)) Ok(40)
None Some(4)
40 Some(40)
6 None Some("hé")
```

- Verdict: native. Indices are usize, and get returns None out of bounds where a[i] would panic. A negative i64 does not convert to usize. checked_sub gives None for the last index of an empty slice. Count from the end with len - k or rev. A str index is a byte range, and get returns None when the range ends inside a character.
