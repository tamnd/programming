```rust run id=impl-trait
// The caller sees only "some iterator of u32", not the adapter types.
fn evens(limit: u32) -> impl Iterator<Item = u32> {
    (0..limit).filter(|n| n % 2 == 0)
}

// A closure has a type with no name, so impl Trait is the way to return it.
fn adder(k: i32) -> impl Fn(i32) -> i32 {
    move |x| x + k
}

fn main() {
    let v: Vec<u32> = evens(10).collect();
    println!("{:?}", v);
    println!("{}", evens(100).map(|n| n * n).sum::<u32>());

    let add5 = adder(5);
    println!("{}", add5(10));
    // let x: std::ops::Range<u32> = evens(10); // error: mismatched types
}
```

```output for=impl-trait
[0, 2, 4, 6, 8]
161700
15
```

- Verdict: native. `impl Iterator<Item = u32>` hides the concrete type, a `Filter` over a `Range` with a closure, so the caller can use only the methods of `Iterator`, and the function can change its body without a change to its signature. The value is not boxed. A closure has a type that no code can write, so `impl Fn` is how a function returns one without a box.
