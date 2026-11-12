// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.

use algo_iterate_over_the_characters_of_a_string::characters;

// A character is a code point. U+1F642 is one character, but two UTF-16 code units and four UTF-8 bytes. A combining mark, a ZWJ and each regional indicator is a character of its own.
#[test]
fn cases() {
    let cases: [(&str, &str, &[&str]); 12] = [
        ("ascii", "abc", &["a", "b", "c"]),
        ("word", "hello", &["h", "e", "l", "l", "o"]),
        ("space", "a b", &["a", " ", "b"]),
        ("empty", "", &[]),
        ("single", "x", &["x"]),
        ("non-ascii", "h\u{e9}", &["h", "\u{e9}"]),
        ("cjk", "\u{65e5}\u{672c}", &["\u{65e5}", "\u{672c}"]),
        ("astral", "a\u{1f642}b", &["a", "\u{1f642}", "b"]),
        ("combining", "e\u{301}", &["e", "\u{301}"]),
        ("regional-indicator", "\u{1f1fa}\u{1f1f8}", &["\u{1f1fa}", "\u{1f1f8}"]),
        ("zwj", "\u{1f469}\u{200d}\u{1f4bb}", &["\u{1f469}", "\u{200d}", "\u{1f4bb}"]),
        ("control", "a\tb\nc", &["a", "\t", "b", "\n", "c"]),
    ];
    for (name, s, want) in cases {
        assert_eq!(characters(s), want, "{name}");
    }
}
