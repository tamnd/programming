// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-substring/.

use algo_get_a_substring::substring;

// U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
#[test]
fn cases() {
    let cases = [
        ("middle", "Hello, world", 7, 12, "world"),
        ("prefix", "Hello, world", 0, 5, "Hello"),
        ("one-char", "abc", 1, 2, "b"),
        ("empty", "", 0, 0, ""),
        ("whole", "abc", 0, 3, "abc"),
        ("empty-range", "abc", 1, 1, ""),
        ("at-end", "abc", 3, 3, ""),
        ("non-ascii", "naïve café", 2, 5, "ïve"),
        ("cjk", "日本語テキスト", 3, 7, "テキスト"),
        ("astral", "a\u{1F642}b\u{1F643}c", 1, 4, "\u{1F642}b\u{1F643}"),
        ("astral-only", "\u{1F642}\u{1F643}", 1, 2, "\u{1F643}"),
        ("combining", "e\u{301}t\u{e9}", 0, 1, "e"),
        ("combining-mark", "e\u{301}t\u{e9}", 1, 3, "\u{301}t"),
    ];
    for (name, s, start, end, want) in cases {
        assert_eq!(substring(s, start, end), want, "{name}");
    }
}
