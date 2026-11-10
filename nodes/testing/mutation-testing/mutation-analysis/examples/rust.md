```rust run id=schema
#[derive(Clone, Copy, PartialEq, Debug)]
enum Mutant {
    None,
    GreaterToGreaterEq,
    GreaterToEq,
    LeftToBest,
    StartAtZero,
}

/// Returns the index of the first largest item of a non-empty slice.
fn argmax(xs: &[i32], m: Mutant) -> usize {
    let mut best = 0;
    let start = if m == Mutant::StartAtZero { 0 } else { 1 };
    for i in start..xs.len() {
        let left = if m == Mutant::LeftToBest { xs[best] } else { xs[i] };
        let bigger = match m {
            Mutant::GreaterToGreaterEq => left >= xs[best],
            Mutant::GreaterToEq => left == xs[best],
            _ => left > xs[best],
        };
        if bigger {
            best = i;
        }
    }
    best
}

fn passes(m: Mutant, suite: &[(&[i32], usize)]) -> bool {
    suite.iter().all(|&(xs, want)| argmax(xs, m) == want)
}

fn main() {
    let weak: &[(&[i32], usize)] = &[(&[1, 2, 3], 2), (&[1, 3, 2], 1), (&[3, 1, 2], 0)];
    let strong: Vec<(&[i32], usize)> = weak.iter().copied().chain([(&[2, 2, 1][..], 0)]).collect();
    assert!(passes(Mutant::None, &strong), "the original must pass first");

    let mutants = [Mutant::GreaterToGreaterEq, Mutant::GreaterToEq, Mutant::LeftToBest, Mutant::StartAtZero];
    println!("{:<20}{:<8}4 tests", "mutant", "3 tests");
    let (mut weak_killed, mut strong_killed) = (0, 0);
    for m in mutants {
        let weak_dead = !passes(m, weak);
        let strong_dead = !passes(m, &strong);
        weak_killed += weak_dead as u32;
        strong_killed += strong_dead as u32;
        let cell = |dead| if dead { "killed" } else { "lived" };
        println!("{:<20}{:<8}{}", format!("{m:?}"), cell(weak_dead), cell(strong_dead));
    }
    println!("{:<20}{weak_killed} of 4  {strong_killed} of 4", "killed");
}
```

```output
mutant              3 tests 4 tests
GreaterToGreaterEq  lived   killed
GreaterToEq         killed  killed
LeftToBest          killed  killed
StartAtZero         lived   lived
killed              2 of 4  3 of 4
```

- Verdict: library. Cargo has no mutation command. cargo-mutants adds `cargo mutants`, which finds places where the code can change and no test fails.
- The second mutant uses a rule of cargo-mutants, which replaces `>` with `==` or `<`. The `match` on the enum is the mutant schema, and the compiler checks that each arm returns a `bool`.
- cargo-mutants also replaces a whole function body with a value of the return type. For `usize` the values are `0` and `1`. The tests above kill both: `[1, 2, 3]` wants `2`.
- cargo-mutants skips functions marked `#[mutants::skip]` and functions under `#[cfg(test)]`.

A mutant that does not compile tells nothing about the tests. Rust has no unary minus for `usize`, so this mutant stops at the compiler.

```rust fails expect=compile id=unviable
/// The mutant puts a unary minus in front of the result.
fn argmax(xs: &[i32]) -> usize {
    let mut best = 0;
    for i in 1..xs.len() {
        if xs[i] > xs[best] {
            best = i;
        }
    }
    -best
}

fn main() {
    println!("{}", argmax(&[1, 3, 2]));
}
```

- cargo-mutants calls such a mutant "unviable". It counts unviable mutants but does not print them by default. It deletes unary operators such as `-a` and `!a`, but it does not replace them with other unary operators, because that gives too many unviable mutants, such as `-false`.
- The type checker does part of the work of the tests here. In Python the same change runs, and the tests must catch the negative result.
