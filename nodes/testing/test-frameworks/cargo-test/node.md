---
tag: T3GMAZ
kind: tool
slug: cargo-test
name: cargo test
aliases: [libtest, Rust test harness]
area: testing/test-frameworks
level: core
summary: >-
  cargo test runs the tests of a Rust package: unit tests, integration tests and doc tests. This page shows #[test], the assert macros, should_panic, ignore, the options and cargo nextest.
role: [test-framework]
licence: mit-or-apache-2.0
repo: https://github.com/rust-lang/cargo
homepage: https://doc.rust-lang.org/cargo/commands/cargo-test.html
see_also: [test-framework, rust, unit-testing, go-test, pytest, node-test]
sources:
  - url: https://doc.rust-lang.org/cargo/commands/cargo-test.html
    title: The Cargo Book, cargo-test(1)
    locator: DESCRIPTION; Documentation tests; Working directory of tests; Test Options; Target Selection; EXAMPLES
    accessed: 2026-11-15
  - url: https://doc.rust-lang.org/rustc/tests/index.html
    title: The rustc book, Tests
    locator: introduction; Test attributes; Filters; Execution options; Output options
    accessed: 2026-11-15
  - url: https://github.com/rust-lang/rust/blob/main/COPYRIGHT
    title: The Rust Project, COPYRIGHT
    locator: Short version for non-lawyers
    accessed: 2026-11-15
  - url: https://doc.rust-lang.org/book/ch11-03-test-organization.html
    title: The Rust Programming Language, Test Organization
    locator: Unit Tests; The tests Module and cfg(test); Integration Tests; Integration Tests for Binary Crates
    accessed: 2026-11-15
  - url: https://doc.rust-lang.org/book/ch11-01-writing-tests.html
    title: The Rust Programming Language, How to Write Tests
    locator: Structuring Test Functions; Checking Results with assert_eq! and assert_ne!; Adding Custom Failure Messages; Checking for Panics with should_panic; Using Result in Tests
    accessed: 2026-11-15
  - url: https://doc.rust-lang.org/book/ch11-02-running-tests.html
    title: The Rust Programming Language, Controlling How Tests Are Run
    locator: Running Tests in Parallel or Consecutively; Showing Function Output; Running a Subset of Tests by Name; Ignoring Tests Unless Specifically Requested
    accessed: 2026-11-15
  - url: https://doc.rust-lang.org/reference/attributes/testing.html
    title: The Rust Reference, Testing attributes
    locator: The test attribute; The ignore attribute; The should_panic attribute
    accessed: 2026-11-15
  - url: https://doc.rust-lang.org/rustdoc/write-documentation/documentation-tests.html
    title: The rustdoc book, Documentation tests
    locator: introduction; Passing or failing a doctest; Pre-processing examples; Attributes
    accessed: 2026-11-15
  - url: https://doc.rust-lang.org/cargo/reference/cargo-targets.html
    title: The Cargo Book, Cargo Targets
    locator: Configuring a target; The test field; The doctest field; The harness field
    accessed: 2026-11-15
  - url: https://nexte.st/
    title: cargo-nextest, Home
    locator: introduction; Quick start
    accessed: 2026-11-15
  - url: https://nexte.st/docs/design/why-process-per-test/
    title: cargo-nextest, Why process-per-test?
    locator: the first paragraph
    accessed: 2026-11-15
  - url: https://crates.io/crates/cargo-nextest
    title: cargo-nextest on crates.io
    locator: versions; licence
    accessed: 2026-11-15
  - url: https://github.com/proptest-rs/proptest
    title: proptest, README
    locator: introduction
    accessed: 2026-11-15
  - url: https://github.com/criterion-rs/criterion.rs
    title: Criterion.rs, README
    locator: Features
    accessed: 2026-11-15
  - url: https://github.com/rust-lang/rust/blob/main/RELEASES.md
    title: The Rust Project, RELEASES.md
    locator: Version 1.0.0; 1.12.0; 1.28.0; 1.39.0; 1.51.0; 1.61.0; 1.85.0; 1.88.0; 1.100.0
    accessed: 2026-11-15
---

## What it is

`cargo test` is the subcommand of Cargo, the package manager of [[rust]], that compiles and runs the tests of a package: unit tests, integration tests and documentation tests.[^1] The test harness is libtest, which the documentation of Cargo calls "rustc's built in unit-test and micro-benchmarking framework".[^1] A Rust project needs no test framework from outside.

`cargo test` builds each test target with the option `--test` of `rustc`. This links the code with libtest, and the test binary runs each function that has the attribute `#[test]`.[^1] The compiler, with `--test`, also makes a `main` function that reads the options of the command line and runs the tests.[^2] Rust and Cargo are open source under the MIT licence and the Apache licence 2.0, at the choice of the user.[^3]

## Install and first use

`cargo test` comes with Rust. The convention for unit tests is a module `tests` with the attribute `#[cfg(test)]` in the same file as the code. The module is compiled only for the tests, so it is not in the normal build.[^4]

```rust test id=first-use
/// A line of a cart: a price in cents and a quantity.
pub struct Item {
    pub cents: u32,
    pub quantity: u32,
}

pub fn total(items: &[Item]) -> u32 {
    items.iter().map(|it| it.cents * it.quantity).sum()
}

pub fn discount(total: u32, code: &str) -> Result<u32, String> {
    match code {
        "TEN" => Ok(total - total / 10),
        "HALF" => Ok(total / 2),
        _ => Err(format!("unknown code {code}")),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn total_adds_the_lines() {
        let items = [Item { cents: 150, quantity: 2 }, Item { cents: 300, quantity: 1 }];
        assert_eq!(total(&items), 600);
    }

    #[test]
    fn empty_cart_is_zero() {
        assert_eq!(total(&[]), 0);
    }

    #[test]
    fn known_codes() {
        assert_eq!(discount(1000, "TEN"), Ok(900));
        assert_ne!(discount(1000, "HALF"), discount(1000, "TEN"));
    }

    #[test]
    fn unknown_code_is_an_error() {
        let result = discount(1000, "FREE");
        assert!(result.is_err(), "got {result:?}");
    }
}
```

This site compiles the block with `rustc --test` and runs the test binary with `--test-threads=1`, so that the tests run one after another. The report gives one line for each test and a summary line.[^2]

```output

running 4 tests
test tests::empty_cart_is_zero ... ok
test tests::known_codes ... ok
test tests::total_adds_the_lines ... ok
test tests::unknown_code_is_an_error ... ok

test result: ok. 4 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in $TIME
```

In a Cargo project, the same code is in `src/lib.rs` or in another file of the package, and the command is `cargo test`.[^1]

## Concepts of the tool

### Assertions and failures

A test fails when it panics.[^2] The macros of the standard library do the checks: `assert!(condition)`, `assert_eq!(left, right)` and `assert_ne!(left, right)`. All three take an optional message, with the same format as `format!`.[^5] The book notes that the two values of `assert_eq!` are called left and right, and that their order does not change the failure message.[^5]

The test below finds a bug: the function divides integers before it converts the result.

```rust test fails expect=test-failure id=failure
/// Gives the mean of the scores. The bug: integer division.
pub fn average(scores: &[u32]) -> f64 {
    let sum: u32 = scores.iter().sum();
    (sum / scores.len() as u32) as f64
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn same_scores() {
        assert_eq!(average(&[4, 4]), 4.0);
    }

    #[test]
    fn half_point() {
        assert_eq!(average(&[2, 3]), 2.5, "the mean of 2 and 3");
    }
}
```

libtest keeps the output of each test. It prints the output of a failed test after the run, with the panic message, the file, the line and the two values.[^6] The other tests of the binary still run.[^1]

```output

running 2 tests
test tests::half_point ... FAILED
test tests::same_scores ... ok

failures:

---- tests::half_point stdout ----

thread 'tests::half_point' ($ID) panicked at main.rs:18:9:
assertion `left == right` failed: the mean of 2 and 3
  left: 2.0
 right: 2.5
note: run with `RUST_BACKTRACE=1` environment variable to display a backtrace


failures:
    tests::half_point

test result: FAILED. 1 passed; 1 failed; 0 ignored; 0 measured; 0 filtered out; finished in $TIME
```

### should_panic, ignore and tests that return Result

The attribute `#[should_panic]` makes a test pass only when the function panics. With the argument `expected`, the panic message must contain the string of the argument.[^7][^5] The attribute `#[ignore]` compiles a test but does not run it by default, and the form `#[ignore = ...]` with a string gives the reason.[^7] A test can also return `Result<(), E>`, where `E` implements `Debug`; the test fails when it returns `Err`, and the body can use the operator `?`.[^5][^7] A test that returns `Result` cannot use `#[should_panic]`.[^5]

```rust test id=attributes
pub fn parse_quantity(text: &str) -> Result<u32, std::num::ParseIntError> {
    text.trim().parse()
}

pub fn set_quantity(quantity: u32) -> u32 {
    if quantity > 99 {
        panic!("quantity {quantity} is more than 99");
    }
    quantity
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parses_with_spaces() -> Result<(), std::num::ParseIntError> {
        assert_eq!(parse_quantity(" 12 ")?, 12);
        Ok(())
    }

    #[test]
    #[should_panic(expected = "more than 99")]
    fn rejects_large_quantity() {
        set_quantity(100);
    }

    #[test]
    #[ignore = "needs the payment service"]
    fn pays_with_a_card() {
        // talks to a service that is not here
    }
}
```

The report marks the `should_panic` test, and gives the reason of the ignored test.

```output

running 3 tests
test tests::parses_with_spaces ... ok
test tests::pays_with_a_card ... ignored, needs the payment service
test tests::rejects_large_quantity - should panic ... ok

test result: ok. 2 passed; 0 failed; 1 ignored; 0 measured; 0 filtered out; finished in $TIME
```

`cargo test -- --ignored` runs only the ignored tests, and `cargo test -- --include-ignored` runs all tests.[^6]

### Unit tests, integration tests and doc tests

A Cargo package has three kinds of tests.[^4][^1]

- Unit tests are in the files of the code, in the module `tests`. They can test private functions, because the rules of privacy in Rust allow it.[^4]
- Integration tests are files in the directory `tests`. Each file is a separate crate that uses the package like any other crate, through its public API, so it needs no `#[cfg(test)]`.[^4] A package with only `src/main.rs` cannot have integration tests of its functions, which is one reason to put the logic in `src/lib.rs`.[^4]
- Documentation tests are the code examples in the documentation comments of the library. rustdoc extracts them and runs them as tests.[^8][^1]

A small package `shop` shows the three kinds. The file `src/lib.rs` has a doc test and a unit test:

```rust test id=lib-unit
/// Gives the price after a discount code, in cents.
///
/// # Examples
///
/// ```
/// assert_eq!(shop::discount(1000, "TEN"), Ok(900));
/// assert!(shop::discount(1000, "FREE").is_err());
/// ```
pub fn discount(total: u32, code: &str) -> Result<u32, String> {
    match code {
        "TEN" => Ok(total - total / 10),
        "HALF" => Ok(total / 2),
        _ => Err(format!("unknown code {code}")),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn half_price() {
        assert_eq!(discount(1000, "HALF"), Ok(500));
    }
}
```

This site compiles the file alone with `rustc --test`, so only the unit test runs. The example in the documentation comment runs with `rustdoc --test` or with `cargo test`.[^8][^1]

```output

running 1 test
test tests::half_price ... ok

test result: ok. 1 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in $TIME
```

The file `tests/checkout.rs` is an integration test. It needs the crate `shop`, so this site shows it as text and does not run it:

```text
// An integration test is a separate crate. It sees only the public API.
use shop::discount;

#[test]
fn two_codes_in_a_row() {
    let once = discount(1000, "TEN").unwrap();
    assert_eq!(discount(once, "HALF"), Ok(450));
}
```

This site runs a single file and cannot run a Cargo package with a `tests` directory. A run of `cargo test` with Rust 1.98.1 on Linux on 15 November 2026 printed the report below. The project directory is written as `/work/shop`.

```text
   Compiling shop v0.1.0 (/work/shop)
    Finished `test` profile [unoptimized + debuginfo] target(s) in 6.41s
     Running unittests src/lib.rs (target/debug/deps/shop-4fe25f01a059bd70)

running 1 test
test tests::half_price ... ok

test result: ok. 1 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

     Running tests/checkout.rs (target/debug/deps/checkout-20b546e3d1678ad2)

running 1 test
test two_codes_in_a_row ... ok

test result: ok. 1 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.01s

   Doc-tests shop

running 1 test
test src/lib.rs - discount (line 5) ... ok

test result: ok. 1 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.01s

all doctests ran in 1.62s; merged doctests compilation took 1.58s
```

Each test target is a separate test binary, and Cargo runs the binaries one after another.[^1] The name of the doc test gives the file, the item and the line of the example. A doc test passes when it compiles and runs with no panic. rustdoc puts the code in a `main` function when the example has none, and adds `extern crate shop;`.[^8] With the edition 2024, rustdoc merges the doc tests that it can into one program before it compiles them, which is faster; each doc test still runs in its own process.[^8]

Code blocks in documentation can have attributes: `ignore` does not compile the block, `no_run` compiles it but does not run it, `should_panic` expects a panic, and `compile_fail` expects a compile error.[^8]

### Parallel tests and output

libtest runs the tests of a binary in parallel on threads by default. The number of threads is the concurrency of the hardware, and `--test-threads` changes it.[^2] The book warns that tests that run at the same time must not depend on each other or on shared state, such as the current directory or environment variables.[^6] libtest captures the standard output of the tests and shows it only for a failed test; `--no-capture` prints it while the tests run, and `--show-output` prints the output of the tests that passed after the run.[^2]

The options of libtest come after the separator `--` on the command line of `cargo test`. The options before it go to Cargo.[^1]

## Common tasks

```text
cargo test                              # all unit, integration and doc tests
cargo test discount                     # tests whose names contain "discount"
cargo test --lib                        # only the unit tests of the library
cargo test --test checkout              # only the integration test tests/checkout.rs
cargo test --doc                        # only the doc tests
cargo test --no-run                     # compile the tests, do not run them
cargo test --no-fail-fast               # run all test binaries, also after a failure
cargo test -- --test-threads=1          # run the tests one after another
cargo test -- --show-output             # print the output of the tests that passed
cargo test -- --ignored                 # run only the ignored tests
cargo test -- --exact tests::half_price # the name must match exactly
cargo test -- --skip slow               # leave out the tests whose names contain "slow"
cargo test -- --list                    # list the tests, do not run them
```

A filter is a part of the test name, so `cargo test half` runs `tests::half_price` and reports the other tests as filtered out.[^6][^2] Without `--no-fail-fast`, Cargo stops after the first test binary that fails, but libtest runs all the tests in that binary.[^1]

## Configuration

The targets of a package are in `Cargo.toml`. For each target, the field `test` tells whether `cargo test` tests it by default, and the field `doctest` of the library turns the doc tests on or off. With `harness = false`, Cargo does not link libtest, and the target must have its own `main` function that runs the tests.[^9][^1]

```toml
# Cargo.toml
[lib]
doctest = false        # do not run the doc tests of the library

[[test]]
name = "checkout"      # tests/checkout.rs
harness = false        # this test has its own main function
```

The working directory of each unit and integration test is the root directory of the package, so a test can open the files of the package with relative paths.[^1]

## Alternatives

### cargo nextest

cargo-nextest is a test runner for Rust from the nextest project. It runs the same `#[test]` functions as `cargo test`, but it runs each test in its own process.[^10][^11] The project calls itself "A next-generation test runner for Rust".[^10] Install it from its pre-built binaries, then run `cargo nextest run`.[^10] It does not run doc tests; its documentation says to run them in a separate step with `cargo test --doc`.[^10] It is under the MIT licence or the Apache licence 2.0, and its version on 12 November 2026 was 0.9.151, from 3 November 2026.[^12]

### Other tools

- proptest: a library for property-based tests in the family of QuickCheck, inspired by Hypothesis. It checks that properties of the code hold for arbitrary inputs, and when it finds a failure, it finds the minimal test case that shows the problem.[^13]
- Criterion.rs: a library for benchmarks with statistics, which runs on stable Rust. The attribute `#[bench]` of libtest is unstable and needs the nightly channel.[^14][^2]
- [[go-test]], [[pytest]] and [[node-test]] are the test tools of other languages that this site describes.

## History and versions

| Version | Change |
|---|---|
| 1.0, 15 May 2015 | the first stable version of Rust[^15] |
| 1.12, 29 September 2016 | the option `--test-threads` of the test binaries[^15] |
| 1.28, 2 August 2018 | test functions can return `Result<(), E: Debug>`[^15] |
| 1.39, 7 November 2019 | the option `--show-output`[^15] |
| 1.51, 25 March 2021 | the option `--include-ignored`[^15] |
| 1.61, 19 May 2022 | the reason of an ignored test is printed in the report[^15] |
| 1.85, 20 February 2025 | the edition 2024, with merged doc tests[^15][^8] |
| 1.88, 26 June 2025 | `--nocapture` is deprecated, and `--no-capture` takes its place[^15] |
| 1.100, 12 November 2026 | the latest version on 12 November 2026[^15] |

The outputs of the runnable blocks on this page come from the Rust version of the site.

## References
