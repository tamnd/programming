// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-function/.

/// Returns value limited to the range from low to high.
pub fn clamp(value: i32, low: i32, high: i32) -> i32 {
    if value < low {
        low
    } else if value > high {
        high
    } else {
        value
    }
}
