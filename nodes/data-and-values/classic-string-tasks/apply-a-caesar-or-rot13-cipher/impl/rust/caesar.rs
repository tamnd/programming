// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-caesar-or-rot13-cipher/.

/// Moves each letter A to Z and a to z `shift` places along the alphabet,
/// from z back to a. Other characters do not change. A negative shift
/// decodes, and `caesar(text, 13)` is ROT13.
pub fn caesar(text: &str, shift: i64) -> String {
    // rem_euclid gives a value from 0 to 25, also for a negative shift.
    let k = u8::try_from(shift.rem_euclid(26)).expect("0 to 25");
    text.chars()
        .map(|c| match c {
            'A'..='Z' => char::from(b'A' + (c as u8 - b'A' + k) % 26),
            'a'..='z' => char::from(b'a' + (c as u8 - b'a' + k) % 26),
            _ => c,
        })
        .collect()
}
