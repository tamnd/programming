// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-a-substring/.

/// Returns s with each match of old replaced by new.
pub fn replace(s: &str, old: &str, new: &str) -> String {
    s.replace(old, new)
}

/// Returns s with the first match of old replaced by new.
pub fn replace_first(s: &str, old: &str, new: &str) -> String {
    // The last argument is the largest number of matches to replace.
    s.replacen(old, new, 1)
}
