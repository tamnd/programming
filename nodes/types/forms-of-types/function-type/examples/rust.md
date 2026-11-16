```rust run id=fn-types
fn inc(n: i32) -> i32 {
    n + 1
}

fn twice(f: fn(i32) -> i32, x: i32) -> i32 {
    f(f(x))
}

fn apply<F: Fn(i32) -> i32>(f: F, x: i32) -> i32 {
    f(x)
}

fn adder(a: i32) -> impl Fn(i32) -> i32 {
    move |b| a + b
}

fn main() {
    println!("{} {}", twice(inc, 5), std::mem::size_of_val(&inc));

    let k = 10;
    let add_k = |n: i32| n + k;
    println!("{}", apply(add_k, 1));

    // Each closure has its own type, so a Vec of closures needs Box<dyn Fn>.
    let ops: Vec<Box<dyn Fn(i32) -> i32>> = vec![Box::new(inc), Box::new(add_k), Box::new(|n| n * n)];
    println!("{:?}", ops.iter().map(|f| f(3)).collect::<Vec<_>>());

    // A closure that captures nothing converts to a fn pointer.
    let square: fn(i32) -> i32 = |n| n * n;
    println!("{} {}", twice(square, 3), adder(10)(5));
}
```

```output for=fn-types
7 0
11
[4, 13, 9]
81 15
```

- Verdict: native. The value `inc` has a function item type of size 0, and it converts to the pointer type `fn(i32) -> i32` when `twice` needs one. A generic parameter with the bound `Fn(i32) -> i32` accepts any closure, and the compiler makes a copy of `apply` for each closure type. `Box<dyn Fn(i32) -> i32>` gives three different functions one type. A closure that captures nothing converts to a `fn` pointer.
