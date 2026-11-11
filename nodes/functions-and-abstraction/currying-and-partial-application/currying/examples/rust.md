```rust run id=closures
fn add(a: i32) -> impl Fn(i32) -> i32 {
    move |b| a + b
}

fn partial<A: Copy, B, R>(f: impl Fn(A, B) -> R, a: A) -> impl Fn(B) -> R {
    move |b| f(a, b)
}

fn main() {
    let add_ten = add(10);
    println!("{} {}", add_ten(5), add(1)(2));

    let clamp_low = partial(i32::max, 0);
    println!("{:?}", [-3, 4, -1].map(clamp_low));
}
```

```output for=closures
15 3
[0, 4, 0]
```

- Verdict: idiom. A function can return a closure with `impl Fn`. The `move` keyword moves the captured values into the closure, so the closure can outlive the function that made it.
- Each closure has its own type. To store closures of different shapes together, use `Box<dyn Fn(i32) -> i32>`.
