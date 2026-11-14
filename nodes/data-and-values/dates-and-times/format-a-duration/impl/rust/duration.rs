// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-duration/.

/// Returns the duration as HH:MM:SS. The hours do not wrap at 24.
/// std::time::Duration has no sign and no format of this kind, so the
/// function takes the seconds as an i64.
pub fn format_duration(seconds: i64) -> String {
    let sign = if seconds < 0 { "-" } else { "" };
    let a = seconds.unsigned_abs();
    format!("{sign}{:02}:{:02}:{:02}", a / 3600, a % 3600 / 60, a % 60)
}
