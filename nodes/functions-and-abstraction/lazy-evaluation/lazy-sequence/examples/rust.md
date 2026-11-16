```rust run id=pipeline
fn square(n: u64) -> u64 {
    println!("square {n}");
    n * n
}

fn main() {
    let odd_squares: Vec<u64> = (1..).map(square).filter(|s| s % 2 == 1).take(2).collect();
    println!("{odd_squares:?}");

    let lazy = (1..=3).map(square);
    println!("built, nothing printed yet");
    let total: u64 = lazy.sum();
    println!("{total}");
}
```

```output for=pipeline
square 1
square 2
square 3
[1, 9]
built, nothing printed yet
square 1
square 2
square 3
14
```

- Verdict: stdlib. Iterator adapters such as `map`, `filter`, `take`, `skip` and `zip` return lazy iterators. `collect`, `sum` and a `for` loop consume them.
- The compiler warns about an adapter chain that is never consumed, with the note "iterators are lazy and do nothing unless consumed".
