// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-strings/.

use algo_concatenate_strings::{concat, concat_number};

#[test]
fn concat_cases() {
    let cases = [
        ("hello-world", "Hello, ", "world", "Hello, world"),
        ("two-words", "good", "bye", "goodbye"),
        ("keep-spaces", "a ", " b", "a  b"),
        ("empty-both", "", "", ""),
        ("empty-left", "", "abc", "abc"),
        ("empty-right", "abc", "", "abc"),
        ("non-ascii", "naï", "ve", "naïve"),
        ("astral", "a", "🙂", "a🙂"),
        ("combining-mark", "e", "\u{301}", "e\u{301}"),
        ("flag-halves", "\u{1f1eb}", "\u{1f1f7}", "🇫🇷"),
        ("digit-strings", "1", "2", "12"),
    ];
    for (name, a, b, want) in cases {
        assert_eq!(concat(a, b), want, "case {name}");
    }
}

#[test]
fn concat_number_cases() {
    let cases = [
        ("positive", "item", 3, "item3"),
        ("zero", "n=", 0, "n=0"),
        ("negative", "x", -5, "x-5"),
        ("empty-prefix", "", 42, "42"),
        ("int32-max", "max=", i32::MAX, "max=2147483647"),
        ("int32-min", "min=", i32::MIN, "min=-2147483648"),
    ];
    for (name, s, n, want) in cases {
        assert_eq!(concat_number(s, n), want, "case {name}");
    }
}
