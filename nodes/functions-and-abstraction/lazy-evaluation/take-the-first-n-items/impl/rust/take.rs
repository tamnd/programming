// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/take-the-first-n-items/.

/// Returns a copy of the first n items; a shorter slice gives all its items.
pub fn take(items: &[i32], n: usize) -> Vec<i32> {
    // Iterator::take stops at the end of the slice, so n can be larger than the length.
    items.iter().take(n).copied().collect()
}

/// Returns the first n items of the infinite sequence start, start + 1, ...
pub fn take_naturals(start: i32, n: usize) -> Vec<i32> {
    // start.. is an infinite range; take reads only n items from it.
    (start..).take(n).collect()
}
