// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pad-a-string/.

/// Returns s padded with fill to width, once on the left and once on the
/// right. If s has width bytes or more, both results are s.
pub fn pad(s: &str, width: usize, fill: &str) -> (String, String) {
    let extra = fill.repeat(width.saturating_sub(s.len()));
    (format!("{extra}{s}"), format!("{s}{extra}"))
}
