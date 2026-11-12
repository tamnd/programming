// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-a-substring/.

use algo_replace_a_substring::{replace, replace_first};

// U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
#[test]
fn all() {
    let cases = [
        ("all", "a-b-c", "-", "+", "a+b+c"),
        ("word", "Hello, world", "world", "there", "Hello, there"),
        ("none", "abc", "x", "y", "abc"),
        ("empty-s", "", "a", "b", ""),
        ("delete", "a-b-c", "-", "", "abc"),
        ("new-contains-old", "aXa", "X", "XXX", "aXXXa"),
        ("overlap", "aaaa", "aa", "b", "bb"),
        ("overlap-odd", "aaa", "aa", "b", "ba"),
        ("case-sensitive", "Hello hello", "hello", "bye", "Hello bye"),
        ("non-ascii", "naïve caf\u{e9}", "\u{e9}", "e", "naïve cafe"),
        ("astral", "a\u{1f642}b\u{1f642}", "\u{1f642}", "!", "a!b!"),
        ("combining", "cafe\u{301}", "e", "E", "cafE\u{301}"),
        ("precomposed", "caf\u{e9}", "e", "E", "caf\u{e9}"),
        ("dot", "a.b.c", ".", "-", "a-b-c"),
        ("dollar", "price", "price", "$&!", "$&!"),
        ("ampersand", "a-b", "-", "&", "a&b"),
        ("backslash", "a/b", "/", "\\", "a\\b"),
    ];
    for (name, s, old, new, want) in cases {
        assert_eq!(replace(s, old, new), want, "{name}");
    }
}

#[test]
fn first() {
    let cases = [
        ("first-only", "a-b-c", "-", "+", "a+b-c"),
        ("first-none", "abc", "x", "y", "abc"),
        ("first-overlap", "aaa", "aa", "b", "ba"),
        ("first-astral", "\u{1f642}\u{1f642}", "\u{1f642}", "x", "x\u{1f642}"),
        ("first-dollar", "a.b", ".", "$&", "a$&b"),
    ];
    for (name, s, old, new, want) in cases {
        assert_eq!(replace_first(s, old, new), want, "{name}");
    }
}
