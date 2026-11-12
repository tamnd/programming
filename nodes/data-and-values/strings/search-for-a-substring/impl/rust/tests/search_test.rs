// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-for-a-substring/.

use algo_search_for_a_substring::{find, find_last};

// U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
#[test]
fn first() {
    let cases = [
        ("found", "Hello, world", "world", 7),
        ("first-of-two", "abcabc", "bc", 1),
        ("not-found", "abc", "d", -1),
        ("empty-sub", "abc", "", 0),
        ("empty-s", "", "a", -1),
        ("both-empty", "", "", 0),
        ("longer-sub", "ab", "abc", -1),
        ("overlap", "aaaa", "aa", 0),
        ("case-sensitive", "Hello", "hello", -1),
        ("non-ascii", "naïve caf\u{e9}", "caf\u{e9}", 6),
        ("cjk", "日本語テキスト", "テキ", 3),
        ("astral", "a\u{1f642}b\u{1f642}c", "b", 2),
        ("astral-sub", "x\u{1f642}y", "\u{1f642}y", 1),
        ("combining", "cafe\u{301}", "e", 3),
        ("precomposed", "cafe\u{301}", "\u{e9}", -1),
    ];
    for (name, s, sub, want) in cases {
        assert_eq!(find(s, sub), want, "{name}");
    }
}

#[test]
fn last() {
    let cases = [
        ("last-of-two", "abcabc", "bc", 4),
        ("last-not-found", "abc", "d", -1),
        ("last-empty-sub", "abc", "", 3),
        ("last-empty-s", "", "", 0),
        ("last-overlap", "aaaa", "aa", 2),
        ("last-astral", "\u{1f642}a\u{1f642}a", "a", 3),
        ("last-non-ascii", "caf\u{e9} caf\u{e9}", "\u{e9}", 8),
    ];
    for (name, s, sub, want) in cases {
        assert_eq!(find_last(s, sub), want, "{name}");
    }
}
