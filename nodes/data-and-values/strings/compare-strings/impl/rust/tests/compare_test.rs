// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-strings/.

use algo_compare_strings::{compare, equals};

// U+FF61 is HALFWIDTH IDEOGRAPHIC FULL STOP. It is before U+1F642 in code point order, but its UTF-16 code unit is after the high surrogate of U+1F642.
#[test]
fn order() {
    let cases = [
        ("less", "apple", "banana", -1),
        ("equal", "apple", "apple", 0),
        ("greater", "pear", "apple", 1),
        ("both-empty", "", "", 0),
        ("empty-first", "", "a", -1),
        ("prefix", "app", "apple", -1),
        ("prefix-longer", "apple", "app", 1),
        ("upper-before-lower", "Zebra", "apple", -1),
        ("lower-after-upper", "a", "A", 1),
        ("digits", "10", "9", -1),
        ("non-ascii", "\u{e9}", "z", 1),
        ("astral-after-bmp", "\u{ff61}", "\u{1f642}", -1),
        ("astral", "\u{1f642}", "\u{1f643}", -1),
        ("combining", "cafe\u{301}", "caf\u{e9}", -1),
    ];
    for (name, a, b, want) in cases {
        assert_eq!(compare(a, b), want, "{name}");
    }
}

#[test]
fn equal() {
    let cases = [
        ("eq-same", "abc", "abc", true),
        ("eq-case", "abc", "ABC", false),
        ("eq-empty", "", "", true),
        ("eq-combining", "cafe\u{301}", "caf\u{e9}", false),
        ("eq-astral", "\u{1f642}", "\u{1f642}", true),
        ("eq-trailing-space", "abc", "abc ", false),
    ];
    for (name, a, b, want) in cases {
        assert_eq!(equals(a, b), want, "{name}");
    }
}
