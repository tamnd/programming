```rust run id=cart flags="--test" args="--test-threads=1"
pub fn cart_total(items: &[(i64, i64)]) -> Result<i64, String> {
    let mut total = 0;
    for &(price, quantity) in items {
        if quantity < 0 {
            return Err(format!("negative quantity: {quantity}"));
        }
        total += price * quantity;
    }
    Ok(total)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn empty_cart_is_zero() {
        assert_eq!(cart_total(&[]), Ok(0));
    }

    #[test]
    fn sums_price_times_quantity() {
        assert_eq!(cart_total(&[(250, 2), (100, 1)]), Ok(600));
    }

    #[test]
    fn rejects_negative_quantity() {
        assert_eq!(
            cart_total(&[(250, -1)]),
            Err("negative quantity: -1".to_string())
        );
    }
}
```

```output for=cart match=regex

running 3 tests
test tests::empty_cart_is_zero ... ok
test tests::rejects_negative_quantity ... ok
test tests::sums_price_times_quantity ... ok

test result: ok. 3 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in \d+\.\d+s
```

- Verdict: native. The `#[test]` attribute is part of the language, and the compiler builds a test harness from the functions that carry it. `cargo test` builds the crate with `rustc --test` and runs the result; this page runs `rustc --test` directly.
- `#[cfg(test)]` compiles the `tests` module only in the test build, so the tests add nothing to the release binary. A unit test in the same file can call private functions of the module.
- The harness runs tests on several threads by default, so the order of the lines can change. With the option `--test-threads=1`, it runs them one at a time in the order of their names.
- A test fails when it panics. `assert_eq!` panics with both values when they differ. The attribute `#[should_panic]` reverses this for a test that must panic.
