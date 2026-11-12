// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-characters-from-a-string/.

/// Returns s without the code points of chars. chars() gives each char, which
/// is a code point.
pub fn remove_chars(s: &str, chars: &str) -> String {
    s.chars().filter(|c| !chars.contains(*c)).collect()
}
