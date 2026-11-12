// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-starts-with-a-prefix/.

use algo_check_whether_a_string_starts_with_a_prefix::{ends_with, starts_with};

// U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
#[test]
fn starts() {
    let cases = [
        ("yes", "Hello, world", "Hello", true),
        ("no", "Hello, world", "world", false),
        ("whole", "abc", "abc", true),
        ("empty-prefix", "abc", "", true),
        ("empty-s", "", "a", false),
        ("both-empty", "", "", true),
        ("longer-prefix", "ab", "abc", false),
        ("case-sensitive", "Hello", "hello", false),
        ("non-ascii", "caf\u{e9} au lait", "caf\u{e9}", true),
        ("cjk", "日本語", "日本", true),
        ("astral", "\u{1f642}\u{1f643}", "\u{1f642}", true),
        ("astral-no", "\u{1f642}", "\u{1f643}", false),
        ("combining", "cafe\u{301}", "cafe", true),
        ("precomposed", "caf\u{e9}", "cafe", false),
    ];
    for (name, s, prefix, want) in cases {
        assert_eq!(starts_with(s, prefix), want, "{name}");
    }
}

#[test]
fn ends() {
    let cases = [
        ("ends-yes", "report.txt", ".txt", true),
        ("ends-no", "report.txt", ".csv", false),
        ("ends-empty", "abc", "", true),
        ("ends-longer", "txt", "a.txt", false),
        ("ends-case", "A.TXT", ".txt", false),
        ("ends-astral", "a\u{1f642}", "\u{1f642}", true),
        ("ends-combining", "cafe\u{301}", "e", false),
    ];
    for (name, s, suffix, want) in cases {
        assert_eq!(ends_with(s, suffix), want, "{name}");
    }
}
