```rust run id=function-composition-rust
fn compose<A, B, C>(f: impl Fn(B) -> C, g: impl Fn(A) -> B) -> impl Fn(A) -> C {
    move |x| f(g(x))
}

fn inc(x: i32) -> i32 {
    x + 1
}

fn twice(x: i32) -> i32 {
    x * 2
}

fn main() {
    println!("{}", compose(twice, inc)(5));
    println!("{}", compose(inc, twice)(5));
    println!("{}", compose(inc, compose(twice, inc))(5));
}
```

```output for=function-composition-rust
12
11
13
```

- Verdict: idiom. The standard library has no compose function. A generic function returns a `move` closure that owns `f` and `g`, and the return type `impl Fn(A) -> C` hides the type of the closure.
