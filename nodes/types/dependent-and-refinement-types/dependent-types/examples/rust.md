```rust run id=const-generics-and-nonzero
use std::num::NonZero;

// The length N is a constant parameter of the type [i32; N]
fn dot<const N: usize>(a: [i32; N], b: [i32; N]) -> i32 {
    a.iter().zip(b.iter()).map(|(x, y)| x * y).sum()
}

// A divisor of type NonZero<u32> cannot be zero, so the division needs no check
fn per_item(total: u32, count: NonZero<u32>) -> u32 {
    total / count
}

fn main() {
    println!("dot: {}", dot([1, 2, 3], [4, 5, 6]));
    println!("dot: {}", dot([1, 2], [3, 4]));
    for n in [4u32, 0] {
        match NonZero::new(n) {
            Some(count) => println!("12 / {} = {}", n, per_item(12, count)),
            None => println!("{} is rejected where the NonZero is made", n),
        }
    }
    println!("size of Option<NonZero<u32>>: {} bytes", size_of::<Option<NonZero<u32>>>());
}
```

```output for=const-generics-and-nonzero
dot: 32
dot: 11
12 / 4 = 3
0 is rejected where the NonZero is made
size of Option<NonZero<u32>>: 4 bytes
```

```rust fails expect=error:E0308 id=lengths-must-match
fn dot<const N: usize>(a: [i32; N], b: [i32; N]) -> i32 {
    a.iter().zip(b.iter()).map(|(x, y)| x * y).sum()
}

fn main() {
    println!("{}", dot([1, 2, 3], [4, 5]));
}
```

- Verdict: idiom. Rust has no dependent types, but const generics let a type depend on a constant value: `dot` takes two arrays of the same length `N`, for any `N`. The fails block passes arrays of lengths 3 and 2, and the compiler rejects the call with E0308, mismatched types. `NonZero<u32>` in the standard library is a refinement of `u32` that the type checks when the value is made, so `per_item` needs no check for zero, and `Option<NonZero<u32>>` uses the zero bit pattern for `None` and fits in 4 bytes.
