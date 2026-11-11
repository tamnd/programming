```rust run id=wrappers
use std::cell::Cell;
use std::fmt::Debug;

fn logged<A: Debug, R: Debug>(name: &'static str, f: impl Fn(A) -> R) -> impl Fn(A) -> R {
    move |a| {
        println!("before {name}({a:?})");
        let r = f(a);
        println!("after {name} -> {r:?}");
        r
    }
}

fn counted<'a, A, R, F: Fn(A) -> R + 'a>(count: &'a Cell<u32>, f: F) -> impl Fn(A) -> R + 'a {
    move |a| {
        count.set(count.get() + 1);
        f(a)
    }
}

fn main() {
    let calls = Cell::new(0);
    let square = counted(&calls, logged("square", |x: i32| x * x));
    square(3);
    square(4);
    println!("calls: {}", calls.get());
}
```

```output for=wrappers
before square(3)
after square -> 9
before square(4)
after square -> 16
calls: 2
```

- Verdict: idiom. Rust has no run-time interception. Higher-order functions that wrap a closure give around advice, and the compiler inlines them, so they cost nothing at run time. Attribute macros give the closest form to AspectJ: the `#[instrument]` attribute of the `tracing` crate rewrites a function to add logging around its body.
