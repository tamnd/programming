// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/swap-two-values/.

/// Returns a copy of `xs` with the items at `i` and `j` swapped.
/// `slice::swap` swaps two items of one slice. The borrow rules do not
/// allow two `&mut` references into the same slice at once, so
/// `std::mem::swap(&mut out[i], &mut out[j])` does not compile.
pub fn swap_items(xs: &[i64], i: usize, j: usize) -> Vec<i64> {
    let mut out = xs.to_vec();
    out.swap(i, j);
    out
}
