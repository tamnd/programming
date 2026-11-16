// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-text-to-a-width/.

use algo_wrap_text_to_a_width::wrap;

const CASES: &[(&str, &str, usize, &[&str])] = &[
    ("fox-ten", "The quick brown fox jumps over the lazy dog", 10, &["The quick", "brown fox", "jumps over", "the lazy", "dog"]),
    ("fox-twenty", "The quick brown fox jumps over the lazy dog", 20, &["The quick brown fox", "jumps over the lazy", "dog"]),
    ("sentence", "Wrap text to a width with a greedy loop.", 15, &["Wrap text to a", "width with a", "greedy loop."]),
    ("empty", "", 10, &[]),
    ("only-spaces", "   ", 5, &[]),
    ("fits", "short text", 20, &["short text"]),
    ("exact-fit", "abcde fghij", 5, &["abcde", "fghij"]),
    ("long-word", "a supercalifragilistic word", 10, &["a", "supercalifragilistic", "word"]),
    ("many-spaces", "one   two\tthree\nfour", 9, &["one two", "three", "four"]),
    ("width-one", "a b c", 1, &["a", "b", "c"]),
    ("line-exact", "ab cd ef", 5, &["ab cd", "ef"]),
];

#[test]
fn cases() {
    for &(name, text, width, want) in CASES {
        assert_eq!(wrap(text, width), want, "{name}");
    }
}
