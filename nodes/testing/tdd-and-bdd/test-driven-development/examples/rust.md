```rust test id=leap
pub fn is_leap(year: u32) -> bool {
    year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)
}

#[cfg(test)]
mod tests {
    use super::*;

    // The tests in the order of the to-do list. Each one added one clause.
    #[test]
    fn t1_divisible_by_4_is_leap() {
        assert!(is_leap(2024));
    }

    #[test]
    fn t2_other_year_is_common() {
        assert!(!is_leap(2023));
    }

    #[test]
    fn t3_century_is_common() {
        assert!(!is_leap(1900));
    }

    #[test]
    fn t4_century_divisible_by_400_is_leap() {
        assert!(is_leap(2000));
    }
}
```

```output

running 4 tests
test tests::t1_divisible_by_4_is_leap ... ok
test tests::t2_other_year_is_common ... ok
test tests::t3_century_is_common ... ok
test tests::t4_century_divisible_by_400_is_leap ... ok

test result: ok. 4 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in $TIME
```

- Verdict: native. The `#[test]` attribute is part of the language, and `cargo test` builds and runs the harness. The page on unit testing covers the harness.
- The harness sorts the tests by name, and the numbers in the names keep the order of the to-do list. With several threads, the lines of the report can come in another order.
- The type `u32` rules out negative years at compile time. A test for `is_leap(-4)` does not compile, so the to-do list needs no item for it.

The first red in Rust is a compile error. A stub moves the red to run time: the macro `todo!()` has every type, so the function compiles, and it panics when a test calls it.

```rust fails expect=compile id=red
fn main() {
    // Red: the test comes first, so is_leap does not exist yet.
    assert!(is_leap(2024));
}
```

```rust fails expect=panic id=stub
fn is_leap(_year: u32) -> bool {
    todo!("the leap year rule")
}

fn main() {
    // The stub compiles, and the test fails when it runs.
    assert!(is_leap(2024));
}
```

- The compiler reports error E0425: cannot find function `is_leap` in this scope.
- `todo!()` panics with the message "not yet implemented", followed by the text in the call. The panic is a red that the test runner can report, unlike the compile error.
