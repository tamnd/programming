// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-code-point-of-a-character/.

use algo_get_the_code_point_of_a_character::code_point;

// U+1F642 is one code point, but it is 2 UTF-16 code units and 4 UTF-8 bytes. U+10FFFF is the largest code point.
#[test]
fn cases() {
    let cases: [(&str, &str, u32); 12] = [
        ("upper", "A", 65),
        ("lower", "a", 97),
        ("digit", "0", 48),
        ("space", " ", 32),
        ("tab", "\t", 9),
        ("tilde", "~", 126),
        ("two-byte", "\u{e9}", 233),
        ("three-byte", "\u{20ac}", 8364),
        ("cjk", "\u{65e5}", 26085),
        ("combining", "\u{301}", 769),
        ("astral", "\u{1f642}", 128578),
        ("max", "\u{10ffff}", 1114111),
    ];
    for (name, c, want) in cases {
        assert_eq!(code_point(c), want, "{name}");
    }
}
