// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/trim-whitespace/.

use algo_trim_whitespace::{trim_start, trim};

// The white space is the 6 ASCII characters: space, tab, line feed, vertical tab, form feed and carriage return. U+00A0, U+3000, U+0085 and U+FEFF stay.
#[test]
fn cases() {
    let cases = [
        ("both-ends", "  hello  ", "hello"),
        ("tab-newline", "\thello world\n", "hello world"),
        ("none", "hello", "hello"),
        ("empty", "", ""),
        ("all-space", " \t\n\r ", ""),
        ("vt-ff", "\x0b\x0cx\x0c\x0b", "x"),
        ("other-control", "\u{1}x\u{1}", "\u{1}x\u{1}"),
        ("no-break-space", "\u{a0}x\u{a0}", "\u{a0}x\u{a0}"),
        ("ideographic-space", "\u{3000}x\u{3000}", "\u{3000}x\u{3000}"),
        ("next-line", "\u{85}x\u{85}", "\u{85}x\u{85}"),
        ("bom", "\u{feff}x", "\u{feff}x"),
        ("astral", " \u{1f642} ", "\u{1f642}"),
        ("combining", " \u{301}x ", "\u{301}x"),
    ];
    for (name, s, want) in cases {
        assert_eq!(trim(s), want, "{name}");
    }
}

#[test]
fn start() {
    let cases = [
        ("start-both-ends", "  hello  ", "hello  "),
        ("start-tab-newline", "\n\tx", "x"),
        ("start-empty", "", ""),
        ("start-all-space", "   ", ""),
        ("start-no-break-space", "\u{a0} x", "\u{a0} x"),
        ("start-end-only", "x \t", "x \t"),
    ];
    for (name, s, want) in cases {
        assert_eq!(trim_start(s), want, "{name}");
    }
}
