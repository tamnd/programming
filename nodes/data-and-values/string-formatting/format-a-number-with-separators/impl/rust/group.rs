// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-separators/.

/// Returns n in decimal with a comma between each group of 3 digits. It
/// converts n to text first, so i64::MIN does not overflow.
pub fn group_thousands(n: i64) -> String {
    let s = n.to_string();
    let (sign, digits) = match s.strip_prefix('-') {
        Some(d) => ("-", d),
        None => ("", s.as_str()),
    };
    let mut out = String::from(sign);
    for (i, c) in digits.chars().enumerate() {
        if i > 0 && (digits.len() - i) % 3 == 0 {
            out.push(',');
        }
        out.push(c);
    }
    out
}
