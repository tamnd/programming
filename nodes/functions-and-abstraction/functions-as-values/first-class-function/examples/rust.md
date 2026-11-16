```rust run id=first-class-function-rust
use std::collections::HashMap;

fn double(x: i32) -> i32 {
    x * 2
}

fn apply_twice(g: fn(i32) -> i32, x: i32) -> i32 {
    g(g(x))
}

fn adder(n: i32) -> impl Fn(i32) -> i32 {
    move |x| x + n
}

fn main() {
    let f: fn(i32) -> i32 = double;
    println!("{}", f(5));

    let mut ops: HashMap<&str, fn(i32) -> i32> = HashMap::new();
    ops.insert("double", double);
    ops.insert("square", |x| x * x);
    println!("{}", ops["square"](4));

    println!("{}", apply_twice(double, 3));

    let add5 = adder(5);
    println!("{}", add5(10));
}
```

```output for=first-class-function-rust
10
16
12
15
```

- Verdict: native. `fn(i32) -> i32` is a function pointer type. A closure that captures nothing coerces to it. The closure from `adder` captures `n`, so its type is written `impl Fn(i32) -> i32`.
