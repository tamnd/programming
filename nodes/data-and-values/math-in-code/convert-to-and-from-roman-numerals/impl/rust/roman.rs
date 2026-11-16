// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

const TABLE: [(i64, &str); 13] = [
    (1000, "M"),
    (900, "CM"),
    (500, "D"),
    (400, "CD"),
    (100, "C"),
    (90, "XC"),
    (50, "L"),
    (40, "XL"),
    (10, "X"),
    (9, "IX"),
    (5, "V"),
    (4, "IV"),
    (1, "I"),
];

/// Returns the Roman numeral of n, or None if n is not in 1 to 3999.
pub fn to_roman(n: i64) -> Option<String> {
    if !(1..=3999).contains(&n) {
        return None;
    }
    let mut n = n;
    let mut s = String::new();
    for (value, symbol) in TABLE {
        while n >= value {
            s.push_str(symbol);
            n -= value;
        }
    }
    Some(s)
}

/// Returns the value of a Roman numeral in the standard form, or None.
pub fn from_roman(s: &str) -> Option<i64> {
    let bytes = s.as_bytes();
    if bytes.is_empty() {
        return None;
    }
    let mut total = 0i64;
    for (i, &c) in bytes.iter().enumerate() {
        let v = value(c)?;
        let next = bytes.get(i + 1).and_then(|&d| value(d)).unwrap_or(0);
        if v < next {
            total -= v;
        } else {
            total += v;
        }
    }
    if to_roman(total)? == s {
        Some(total)
    } else {
        None
    }
}

fn value(c: u8) -> Option<i64> {
    match c {
        b'I' => Some(1),
        b'V' => Some(5),
        b'X' => Some(10),
        b'L' => Some(50),
        b'C' => Some(100),
        b'D' => Some(500),
        b'M' => Some(1000),
        _ => None,
    }
}
