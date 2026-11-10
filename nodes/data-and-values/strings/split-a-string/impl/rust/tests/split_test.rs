// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/split-a-string/.

use algo_split_a_string::{split, split_whitespace};

#[test]
fn split_cases() {
    let cases: &[(&str, &str, &str, usize, &[&str])] = &[
        ("comma", "a,b,c", ",", 0, &["a", "b", "c"]),
        ("csv-line", "Ada,1815,London", ",", 0, &["Ada", "1815", "London"]),
        ("multi-char-separator", "a::b::c", "::", 0, &["a", "b", "c"]),
        ("space-separator", "one two  three", " ", 0, &["one", "two", "", "three"]),
        ("limit-two", "key=value=more", "=", 2, &["key", "value=more"]),
        ("empty", "", ",", 0, &[""]),
        ("no-separator", "abc", ",", 0, &["abc"]),
        ("leading-separator", ",a,b", ",", 0, &["", "a", "b"]),
        ("trailing-separators", "a,b,,", ",", 0, &["a", "b", "", ""]),
        ("adjacent-separators", "a,,b", ",", 0, &["a", "", "b"]),
        ("only-separator", ",", ",", 0, &["", ""]),
        ("overlapping-match", "aaa", "aa", 0, &["", "a"]),
        ("dot-separator", "192.168.0.1", ".", 0, &["192", "168", "0", "1"]),
        ("pipe-separator", "a|b|c", "|", 0, &["a", "b", "c"]),
        ("non-ascii", "día→naïve→日本→🙂", "→", 0, &["día", "naïve", "日本", "🙂"]),
        ("limit-one", "a,b,c", ",", 1, &["a,b,c"]),
        ("limit-above-count", "a,b", ",", 5, &["a", "b"]),
        ("limit-keeps-trailing-empty", "a,b,", ",", 3, &["a", "b", ""]),
    ];
    for &(name, s, sep, limit, want) in cases {
        assert_eq!(split(s, sep, limit), want, "case {name}");
    }
}

#[test]
fn split_whitespace_cases() {
    let cases: &[(&str, &str, &[&str])] = &[
        ("single-spaces", "a b c", &["a", "b", "c"]),
        ("runs-and-ends", "  lead  and trail  ", &["lead", "and", "trail"]),
        ("tabs-and-newlines", "a\t\tb\nc\n", &["a", "b", "c"]),
        ("one-word", "word", &["word"]),
        ("empty", "", &[]),
        ("only-white-space", " \t\n ", &[]),
    ];
    for &(name, s, want) in cases {
        assert_eq!(split_whitespace(s), want, "case {name}");
    }
}
