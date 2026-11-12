// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/repeat-a-string/.

/// Returns n copies of s. str::repeat panics if the capacity overflows.
pub fn repeat(s: &str, n: usize) -> String {
    s.repeat(n)
}
