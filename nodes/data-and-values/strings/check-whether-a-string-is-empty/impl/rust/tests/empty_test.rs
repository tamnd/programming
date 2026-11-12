// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

use algo_check_whether_a_string_is_empty::{is_blank, is_empty};

// U+00A0 is NO-BREAK SPACE, U+200B is ZERO WIDTH SPACE, and U+3000 is IDEOGRAPHIC SPACE. None of them is ASCII white space.
#[test]
fn empty() {
    let cases = [
        ("letter", "a", false),
        ("word", "hello", false),
        ("empty", "", true),
        ("space", " ", false),
        ("newline", "\n", false),
        ("nbsp", "\u{a0}", false),
        ("zero-width", "\u{200b}", false),
        ("combining", "\u{301}", false),
        ("astral", "\u{1f642}", false),
        ("cjk", "日", false),
    ];
    for (name, s, want) in cases {
        assert_eq!(is_empty(s), want, "{name}");
    }
}

#[test]
fn blank() {
    let cases = [
        ("blank-empty", "", true),
        ("blank-space", " ", true),
        ("blank-mixed", " \t\n\r", true),
        ("blank-vt-ff", "\x0b\x0c", true),
        ("blank-letter", "a", false),
        ("blank-padded", "  a  ", false),
        ("blank-nbsp", "\u{a0}", false),
        ("blank-zero-width", "\u{200b}", false),
        ("blank-ideographic", "\u{3000}", false),
        ("blank-astral", " \u{1f642} ", false),
    ];
    for (name, s, want) in cases {
        assert_eq!(is_blank(s), want, "{name}");
    }
}
