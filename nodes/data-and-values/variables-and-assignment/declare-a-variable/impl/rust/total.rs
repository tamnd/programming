// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-variable/.

/// Returns the sum of `values`. A `let` variable is immutable unless it
/// has `mut`, and `result` gets a new value on each pass, so it needs
/// `mut`. The annotation `: i64` is optional here, because the compiler
/// infers the type from the return type.
pub fn total(values: &[i64]) -> i64 {
    let mut result: i64 = 0;
    for &value in values {
        result = result + value;
    }
    result
}
