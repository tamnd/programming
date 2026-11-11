```rust run id=continuations
// A boxed closure, so every step has the same type.
fn fact(n: u64, k: Box<dyn FnOnce(u64) -> u64>) -> u64 {
    if n == 0 {
        k(1)
    } else {
        fact(n - 1, Box::new(move |r| k(n * r)))
    }
}

// Two continuations: one for success, one for failure.
fn parse<R>(s: &str, ok: impl FnOnce(i32) -> R, fail: impl FnOnce(String) -> R) -> R {
    match s.parse::<i32>() {
        Ok(n) => ok(n),
        Err(e) => fail(format!("{s}: {e}")),
    }
}

fn main() {
    println!("{}", fact(5, Box::new(|r| r)));
    println!("{}", fact(20, Box::new(|r| r)));
    for s in ["42", "4x"] {
        let msg = parse(s, |n| format!("ok {}", n + 1), |e| format!("error: {e}"));
        println!("{msg}");
    }
}
```

```output for=continuations
120
2432902008176640000
ok 43
error: 4x: invalid digit found in string
```

```rust fails expect=compile id=generic-recursion
// Each call makes a new closure type, so the compiler must make a new
// copy of fact for every level, without end.
fn fact<K: FnOnce(u64) -> u64>(n: u64, k: K) -> u64 {
    if n == 0 {
        k(1)
    } else {
        fact(n - 1, move |r| k(n * r))
    }
}

fn main() {
    println!("{}", fact(5, |r| r));
}
```

- Verdict: idiom. Closures in Rust capture their environment and implement `FnOnce`, so a function can take its continuation as a parameter, as `parse` does. A generic parameter costs nothing at run time, because the compiler makes a copy of `parse` for each pair of closures and can inline them.
- Recursive CPS needs a box. The failing block makes `fact` generic in its continuation, but each level wraps `k` in a new closure, which is a new type, and the compiler stops when it reaches its limit for nested copies. `Box<dyn FnOnce>` gives all the continuations one type, at the cost of a heap allocation and an indirect call for each step.
