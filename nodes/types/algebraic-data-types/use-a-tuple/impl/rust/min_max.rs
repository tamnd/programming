// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-tuple/.

/// Returns (smallest, largest) of a slice with at least one number.
pub fn min_max(numbers: &[i32]) -> (i32, i32) {
    let mut pair = (numbers[0], numbers[0]);
    for &n in numbers {
        pair.0 = pair.0.min(n);
        pair.1 = pair.1.max(n);
    }
    pair
}
