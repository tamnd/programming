// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

use algo_convert_to_and_from_roman_numerals::{from_roman, to_roman};

const CASES: &[(&str, i64, &str)] = &[
    ("year-1990", 1990, "MCMXC"),
    ("year-2008", 2008, "MMVIII"),
    ("each-symbol-once", 1666, "MDCLXVI"),
    ("one", 1, "I"),
    ("four", 4, "IV"),
    ("nine", 9, "IX"),
    ("forty-nine", 49, "XLIX"),
    ("ninety-nine", 99, "XCIX"),
    ("four-hundred-forty-four", 444, "CDXLIV"),
    ("year-1994", 1994, "MCMXCIV"),
    ("longest", 3888, "MMMDCCCLXXXVIII"),
    ("largest", 3999, "MMMCMXCIX"),
];

#[test]
fn cases() {
    for &(name, n, want) in CASES {
        assert_eq!(to_roman(n).as_deref(), Some(want), "{name}");
        assert_eq!(from_roman(want), Some(n), "{name}");
    }
}

#[test]
fn round_trip() {
    for n in 1..=3999 {
        let s = to_roman(n).expect("in range");
        assert_eq!(from_roman(&s), Some(n));
    }
}

#[test]
fn invalid() {
    for s in ["", "IIII", "IC", "VX", "MMMM", "iv", "ABC", "IIV", "XM"] {
        assert_eq!(from_roman(s), None, "{s}");
    }
    for n in [0, -1, 4000] {
        assert_eq!(to_roman(n), None);
    }
}
