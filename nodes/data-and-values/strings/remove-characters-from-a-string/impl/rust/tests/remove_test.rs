// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-characters-from-a-string/.

use algo_remove_characters_from_a_string::remove_chars;

// A character is a code point. U+1F642 and U+1F643 have the same high surrogate in UTF-16, and U+00E0 and U+00E9 have the same first byte in UTF-8.
#[test]
fn cases() {
    let cases: [(&str, &str, &str, &str); 14] = [
        ("one-char", "banana", "a", "bnn"),
        ("set", "hello, world", "lo", "he, wrd"),
        ("punct", "a-b_c.d", "-_.", "abcd"),
        ("empty", "", "abc", ""),
        ("empty-set", "hello", "", "hello"),
        ("none-found", "hello", "xyz", "hello"),
        ("all", "aaa", "a", ""),
        ("adjacent", "aab", "a", "b"),
        ("astral", "a\u{1f642}b\u{1f642}", "\u{1f642}", "ab"),
        ("astral-other", "\u{1f642}\u{1f643}", "\u{1f643}", "\u{1f642}"),
        ("non-ascii", "\u{e0}\u{e9}", "\u{e9}", "\u{e0}"),
        ("combining-mark", "e\u{301}x", "\u{301}", "ex"),
        ("combining-base", "e\u{301}", "e", "\u{301}"),
        ("composed", "e\u{301}", "\u{e9}", "e\u{301}"),
    ];
    for (name, s, chars, want) in cases {
        assert_eq!(remove_chars(s, chars), want, "{name}");
    }
}
