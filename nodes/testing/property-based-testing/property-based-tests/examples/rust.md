```rust run id=checker
use std::fmt::Debug;

/// xorshift32 with Marsaglia's seed: the same numbers on every run.
struct Rng(u32);

impl Rng {
    fn next(&mut self) -> u32 {
        self.0 ^= self.0 << 13;
        self.0 ^= self.0 >> 17;
        self.0 ^= self.0 << 5;
        self.0
    }

    fn between(&mut self, lo: i64, hi: i64) -> i64 {
        lo + (self.next() as u64 % (hi - lo + 1) as u64) as i64
    }
}

/// Runs a property on 100 generated cases and shrinks the first failure.
fn check<T: Debug>(
    name: &str,
    generate: impl Fn(&mut Rng) -> T,
    holds: impl Fn(&T) -> bool,
    shrink: impl Fn(&T) -> Vec<T>,
) {
    let mut rng = Rng(2463534242);
    for trial in 1..=100 {
        let mut case = generate(&mut rng);
        if holds(&case) {
            continue;
        }
        println!("{name}: falsified on test {trial} by {case:?}");
        let mut steps = 0;
        while let Some(smaller) = shrink(&case).into_iter().find(|c| !holds(c)) {
            case = smaller;
            steps += 1;
        }
        println!("{name}: shrunk in {steps} steps to {case:?}");
        return;
    }
    println!("{name}: passed 100 tests");
}

/// Gives lo, then x - d for d = (x - lo) / 2, / 4, ..., 1.
fn shrink_int(x: u32, lo: u32) -> Vec<u32> {
    let mut out = Vec::new();
    if x > lo {
        out.push(lo);
    }
    let mut d = (x - lo) / 2;
    while d > 0 {
        out.push(x - d);
        d /= 2;
    }
    out
}

fn pages(items: u32, per_page: u32) -> u32 {
    items / per_page // bug: drops the last page when it is not full
}

fn main() {
    check(
        "round trip",
        |rng| rng.between(i32::MIN as i64, i32::MAX as i64) as i32,
        |x| x.to_string().parse::<i32>() == Ok(*x),
        |_| Vec::new(),
    );

    check(
        "pages",
        |rng| (rng.between(0, 1000) as u32, rng.between(1, 50) as u32),
        |&(items, per_page)| pages(items, per_page) * per_page >= items,
        |&(items, per_page)| {
            let mut out: Vec<(u32, u32)> = shrink_int(items, 0).into_iter().map(|n| (n, per_page)).collect();
            out.extend(shrink_int(per_page, 1).into_iter().map(|p| (items, p)));
            out
        },
    );
}
```

```output
round trip: passed 100 tests
pages: falsified on test 1 by (967, 7)
pages: shrunk in 13 steps to (1, 2)
```

The first fix for the bug rounds up with `(items + per_page - 1) / per_page`. Near the top of the `u32` range the sum overflows. A uniform generator almost never comes close to the top, so this generator takes a value near `u32::MAX` on one test in four.

```rust fails expect=panic id=overflow
struct Rng(u32);

impl Rng {
    fn next(&mut self) -> u32 {
        self.0 ^= self.0 << 13;
        self.0 ^= self.0 >> 17;
        self.0 ^= self.0 << 5;
        self.0
    }

    /// One test in four takes a value near the maximum.
    fn edgy_u32(&mut self) -> u32 {
        if self.next() % 4 == 0 { u32::MAX - self.next() % 4 } else { self.next() }
    }
}

fn pages(items: u32, per_page: u32) -> u32 {
    (items + per_page - 1) / per_page // a debug build panics when the sum overflows
}

fn main() {
    let mut rng = Rng(2463534242);
    for _ in 0..100 {
        let (items, per_page) = (rng.edgy_u32(), 1 + rng.next() % 50);
        assert!(u64::from(pages(items, per_page)) * u64::from(per_page) >= u64::from(items));
    }
    println!("passed 100 tests");
}
```

A debug build panics with `attempt to add with overflow`, and the property test has found a second bug. `div_ceil` rounds up with no intermediate sum. The second assertion checks that the count is not too large.

```rust run id=div-ceil
struct Rng(u32);

impl Rng {
    fn next(&mut self) -> u32 {
        self.0 ^= self.0 << 13;
        self.0 ^= self.0 >> 17;
        self.0 ^= self.0 << 5;
        self.0
    }

    /// One test in four takes a value near the maximum.
    fn edgy_u32(&mut self) -> u32 {
        if self.next() % 4 == 0 { u32::MAX - self.next() % 4 } else { self.next() }
    }
}

fn pages(items: u32, per_page: u32) -> u32 {
    items.div_ceil(per_page) // rounds up with no intermediate sum
}

fn main() {
    let mut rng = Rng(2463534242);
    let mut near_max = 0;
    for _ in 0..100 {
        let (items, per_page) = (rng.edgy_u32(), 1 + rng.next() % 50);
        let n = pages(items, per_page);
        assert!(u64::from(n) * u64::from(per_page) >= u64::from(items), "pages too few");
        assert!(n == 0 || u64::from(n - 1) * u64::from(per_page) < u64::from(items), "pages too many");
        near_max += (items > u32::MAX - 4) as u32;
    }
    println!("passed 100 tests, {near_max} of them near u32::MAX");
}
```

```output for=div-ceil
passed 100 tests, 28 of them near u32::MAX
```

- Verdict: library. proptest is a property testing library in the QuickCheck family. Its strategies generate and shrink each value.
- `check` takes three closures through `impl Fn`. The bound `T: Debug` lets it print any case with `{:?}`, so a tuple prints as `(1, 2)`.
- `while let Some(smaller) = ... .find(...)` is the whole shrink loop: it stops when no candidate fails.
- Arithmetic overflow panics in a debug build, and a release build wraps. The C example shows the same bug with no panic.
- The property computes `pages * per_page` in `u64`, so the check itself cannot overflow.
