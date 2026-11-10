```rust run id=folded
use std::cell::RefCell;
use std::collections::BTreeMap;

thread_local! {
    static STACK: RefCell<Vec<&'static str>> = const { RefCell::new(Vec::new()) };
    static FOLDED: RefCell<BTreeMap<String, u64>> = const { RefCell::new(BTreeMap::new()) };
}

/// Manual instrumentation: a Scope records the path of calls that led to it,
/// one line per path, as a flame graph expects. Drop pops the frame.
struct Scope;

impl Scope {
    fn enter(name: &'static str) -> Scope {
        let path = STACK.with_borrow_mut(|stack| {
            stack.push(name);
            stack.join(";")
        });
        FOLDED.with_borrow_mut(|folded| *folded.entry(path).or_insert(0) += 1);
        Scope
    }
}

impl Drop for Scope {
    fn drop(&mut self) {
        STACK.with_borrow_mut(|stack| stack.pop());
    }
}

fn fib(n: u32) -> u64 {
    let _scope = Scope::enter("fib");
    if n < 2 { n as u64 } else { fib(n - 1) + fib(n - 2) }
}

fn square(x: u64) -> u64 {
    let _scope = Scope::enter("square");
    x * x
}

fn main() {
    {
        let _scope = Scope::enter("main");
        println!("fib(4) = {}", fib(4));
        println!("square(fib(5)) = {}", square(fib(5)));
    }
    FOLDED.with_borrow(|folded| {
        for (path, count) in folded {
            println!("{path} {count}");
        }
    });
}
```

```output for=folded
fib(4) = 3
square(fib(5)) = 25
main 1
main;fib 2
main;fib;fib 4
main;fib;fib;fib 8
main;fib;fib;fib;fib 8
main;fib;fib;fib;fib;fib 2
main;square 1
```

- Verdict: library. The Rust standard library has no profiler. A tool outside the language, such as `perf` on Linux, samples a compiled Rust program as it samples a C program.
- `Drop` pops the frame when the guard goes out of scope, also on an early return and on a panic that unwinds.
- `BTreeMap` keeps the paths sorted, so the output has a fixed order.
