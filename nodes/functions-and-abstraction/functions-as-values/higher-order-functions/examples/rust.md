```rust run id=hof
fn twice(f: impl Fn(i32) -> i32, x: i32) -> i32 {
    f(f(x))
}

fn compose<A, B, C>(f: impl Fn(B) -> C, g: impl Fn(A) -> B) -> impl Fn(A) -> C {
    move |x| f(g(x))
}

fn apply_all(fs: &[fn(i32) -> i32], x: i32) -> Vec<i32> {
    fs.iter().map(|f| f(x)).collect()
}

fn main() {
    let words = vec!["pear", "fig", "banana"];
    println!("{}", twice(|n| n * 3, 2));
    let lengths: Vec<usize> = words.iter().map(|w| w.len()).collect();
    println!("{lengths:?}");
    let long: Vec<&&str> = words.iter().filter(|w| w.len() > 3).collect();
    println!("{long:?}");
    println!("{}", [1, 2, 3, 4].iter().fold(0, |total, n| total + n));
    let mut sorted = words.clone();
    sorted.sort_by_key(|w| w.len());
    println!("{sorted:?}");
    let add_then_double = compose(|n: i32| n * 2, |n: i32| n + 1);
    println!("{}", add_then_double(5));
    println!("{:?}", apply_all(&[|n| n + 1, |n| n * n], 4));
}
```

```output for=hof
18
[4, 3, 6]
["pear", "banana"]
10
["fig", "pear", "banana"]
12
[5, 16]
```

- Verdict: native. Closures implement the traits `Fn`, `FnMut` or `FnOnce`. A generic parameter such as `impl Fn(i32) -> i32` accepts any closure with that signature.
- Each closure has its own type, so the compiler makes a separate copy of `twice` for each closure type. It can then inline the call.
- The type `fn(i32) -> i32` is a function pointer. Named functions and closures that capture nothing convert to it, as in `apply_all`.
- Iterator adapters such as `map` and `filter` are lazy. `collect` or `fold` runs them.
