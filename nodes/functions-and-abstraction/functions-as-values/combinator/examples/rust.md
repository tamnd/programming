```rust run id=combinator-rust
use std::convert::identity;

fn constant<A: Copy, B>(x: A) -> impl Fn(B) -> A {
    move |_| x
}

fn flip<A, B, C>(f: impl Fn(A, B) -> C) -> impl Fn(B, A) -> C {
    move |b, a| f(a, b)
}

fn both<T: Copy>(p: impl Fn(T) -> bool, q: impl Fn(T) -> bool) -> impl Fn(T) -> bool {
    move |x| p(x) && q(x)
}

fn main() {
    println!("{}", identity(5));
    println!("{}", constant::<i32, i32>(7)(99));
    let sub = |a: i32, b: i32| a - b;
    println!("{}", flip(sub)(2, 10));
    let keep = both(|x: i32| x % 2 == 0, |x: i32| x > 0);
    let kept: Vec<String> = (-2..=4).filter(|&x| keep(x)).map(|x| x.to_string()).collect();
    println!("{}", kept.join(" "));
}
```

```output for=combinator-rust
5
7
8
2 4
```

- Verdict: stdlib. `std::convert::identity` is in the library, and `Option` and `Iterator` have many combinator methods. A function that returns `impl Fn` gives `constant`, `flip` and `both`.
