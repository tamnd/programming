// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/map-over-a-list/.

/// Returns a new vector with the square of each item, in the same order.
pub fn square_all(items: &[i32]) -> Vec<i32> {
    // map is lazy. collect runs it and makes the vector.
    items.iter().map(|x| x * x).collect()
}
