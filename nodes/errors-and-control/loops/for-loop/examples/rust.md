```rust run id=ranges-rev-and-step-by
fn main() {
    // A half-open range does not include its end; an inclusive range does.
    let a: Vec<i32> = (0..5).collect();
    let b: Vec<i32> = (1..=5).collect();
    println!("{a:?} {b:?}");

    // rev counts down and step_by counts by a step.
    let down: Vec<i32> = (1..=5).rev().collect();
    let odd: Vec<i32> = (1..10).step_by(2).collect();
    println!("{down:?} {odd:?}");

    for i in (1..=3).rev() {
        println!("countdown {i}");
    }

    // The loop variable is a new immutable binding for each pass.
    let mut total = 0;
    for i in 1..=100 {
        total += i;
    }
    println!("total: {total}");
}
```

```output for=ranges-rev-and-step-by
[0, 1, 2, 3, 4] [1, 2, 3, 4, 5]
[5, 4, 3, 2, 1] [1, 3, 5, 7, 9]
countdown 3
countdown 2
countdown 1
total: 5050
```

- Verdict: idiom. Rust has no three-part loop. A counted loop is a `for` over a range: `0..5` leaves out 5 and `1..=5` includes it. `rev` turns a range around and `step_by(2)` takes every second value. The loop variable is a new binding in each pass, and the body cannot change it.
