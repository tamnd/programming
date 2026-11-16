// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-conditional/.

/// Returns the letter grade for a score from 0 to 100.
pub fn grade(score: i32) -> &'static str {
    // if is an expression, so the chain is the value of the function.
    if score >= 90 {
        "A"
    } else if score >= 80 {
        "B"
    } else if score >= 70 {
        "C"
    } else if score >= 60 {
        "D"
    } else {
        "F"
    }
}
