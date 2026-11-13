// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

/// Returns the sum of `items`. The sum of an empty slice is 0.
pub fn total(items: &[i32]) -> i32 {
    // The i64 accumulator holds each partial sum of i32 items.
    let sum = items.iter().fold(0i64, |acc, &x| acc + i64::from(x));
    i32::try_from(sum).expect("total: the sum does not fit in an i32")
}
