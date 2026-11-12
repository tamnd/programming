// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.

/// Returns the code points of `s`. `chars()` gives each `char`, which is a
/// code point. `bytes()` would give the UTF-8 bytes.
pub fn characters(s: &str) -> Vec<String> {
    s.chars().map(String::from).collect()
}
