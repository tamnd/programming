// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/change-the-case-of-a-string/.

use algo_change_the_case_of_a_string::{to_lower, to_upper};

// Only the ASCII letters change. U+0131 is the dotless i, U+0130 is the capital I with a dot, U+212A is the KELVIN SIGN, and U+FF41 is the fullwidth a.
#[test]
fn upper() {
    let cases = [
        ("word", "hello", "HELLO"),
        ("mixed", "Hello, World!", "HELLO, WORLD!"),
        ("already", "ABC", "ABC"),
        ("empty", "", ""),
        ("digits-punct", "a1-b2_c3", "A1-B2_C3"),
        ("non-ascii", "caf\u{e9}", "CAF\u{e9}"),
        ("sharp-s", "stra\u{df}e", "STRA\u{df}E"),
        ("dotless-i", "\u{131} i", "\u{131} I"),
        ("greek", "\u{3c3}", "\u{3c3}"),
        ("fullwidth", "\u{ff41}", "\u{ff41}"),
        ("astral", "a\u{1f642}b", "A\u{1f642}B"),
        ("combining", "cafe\u{301}", "CAFE\u{301}"),
    ];
    for (name, s, want) in cases {
        assert_eq!(to_upper(s), want, "{name}");
    }
}

#[test]
fn lower() {
    let cases = [
        ("lower-word", "HELLO", "hello"),
        ("lower-mixed", "Hello, World!", "hello, world!"),
        ("lower-empty", "", ""),
        ("lower-dotted-i", "\u{130}", "\u{130}"),
        ("lower-sigma", "\u{3a3}\u{391}\u{3a3}", "\u{3a3}\u{391}\u{3a3}"),
        ("lower-kelvin", "\u{212a}", "\u{212a}"),
        ("lower-astral", "A\u{1f642}B", "a\u{1f642}b"),
    ];
    for (name, s, want) in cases {
        assert_eq!(to_lower(s), want, "{name}");
    }
}
