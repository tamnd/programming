// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/join-a-list-of-strings/.

use algo_join_a_list_of_strings::{join, join_numbers};

#[test]
fn join_cases() {
    let cases: &[(&str, &[&str], &str, &str)] = &[
        ("comma", &["a", "b", "c"], ",", "a,b,c"),
        ("comma-space", &["red", "green", "blue"], ", ", "red, green, blue"),
        ("words", &["Hello", "world"], " ", "Hello world"),
        ("empty-list", &[], ",", ""),
        ("single", &["only"], ",", "only"),
        ("empty-items", &["", "", ""], ",", ",,"),
        ("empty-middle", &["a", "", "b"], "-", "a--b"),
        ("empty-sep", &["a", "b", "c"], "", "abc"),
        ("long-sep", &["x", "y"], " -> ", "x -> y"),
        ("non-ascii", &["naïve", "日本"], "·", "naïve·日本"),
        ("astral-sep", &["a", "b"], "\u{1F642}", "a\u{1F642}b"),
        ("sep-in-item", &["a,b", "c"], ",", "a,b,c"),
    ];
    for &(name, items, sep, want) in cases {
        assert_eq!(join(items, sep), want, "{name}");
    }
}

#[test]
fn join_numbers_cases() {
    let cases: &[(&str, &[i32], &str, &str)] = &[
        ("numbers", &[1, 2, 3], ",", "1,2,3"),
        ("negative", &[-1, 0, 1], " ", "-1 0 1"),
        ("empty", &[], ",", ""),
        ("single", &[42], ",", "42"),
        ("int32-ends", &[2147483647, -2147483648], ";", "2147483647;-2147483648"),
    ];
    for &(name, items, sep, want) in cases {
        assert_eq!(join_numbers(items, sep), want, "{name}");
    }
}
