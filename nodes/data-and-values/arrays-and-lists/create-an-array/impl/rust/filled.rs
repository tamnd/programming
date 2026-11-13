// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-an-array/.

/// Returns a new vector of n items, and each item is value.
pub fn filled(n: usize, value: i32) -> Vec<i32> {
    // vec! clones value n times. For i32 a clone is a copy.
    vec![value; n]
}
