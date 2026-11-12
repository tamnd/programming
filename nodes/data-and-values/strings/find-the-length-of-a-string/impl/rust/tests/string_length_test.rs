// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-length-of-a-string/.

use algo_find_the_length_of_a_string::lengths;

#[test]
fn cases() {
    let cases: &[(&str, &str, [usize; 3])] = &[
        ("ascii", "hello", [5, 5, 5]),
        ("words", "Hello, world", [12, 12, 12]),
        ("two-byte", "naïve", [6, 5, 5]),
        ("three-byte", "日本語", [9, 3, 3]),
        ("cyrillic", "Привет", [12, 6, 6]),
        ("empty", "", [0, 0, 0]),
        ("single", "a", [1, 1, 1]),
        ("euro-sign", "€", [3, 1, 1]),
        ("astral", "🙂", [4, 2, 1]),
        ("mixed", "a🙂b", [6, 4, 3]),
        ("combining-mark", "e\u{301}", [3, 2, 2]),
        ("precomposed", "\u{e9}", [2, 1, 1]),
        ("flag", "🇫🇷", [8, 4, 2]),
        ("zwj-family", "👨\u{200d}👩\u{200d}👧", [18, 8, 5]),
        ("nul", "a\0b", [3, 3, 3]),
    ];
    for (name, s, want) in cases {
        assert_eq!(lengths(s), *want, "case {name}");
    }
}
