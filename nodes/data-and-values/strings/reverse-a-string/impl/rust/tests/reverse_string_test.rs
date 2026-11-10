// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-string/.

use algo_reverse_a_string::reverse_code_points;

#[test]
fn cases() {
    let cases = [
        ("ascii", "hello", "olleh"),
        ("two-words", "abc def", "fed cba"),
        ("palindrome", "racecar", "racecar"),
        ("empty", "", ""),
        ("single", "a", "a"),
        ("two-byte", "naïve", "evïan"),
        ("three-byte", "日本語", "語本日"),
        ("astral", "a🙂b", "b🙂a"),
        ("two-astral", "🙂😀", "😀🙂"),
        ("combining-mark", "e\u{301}x", "x\u{301}e"),
        ("flag", "🇫🇷", "🇷🇫"),
    ];
    for (name, s, want) in cases {
        let got = reverse_code_points(s);
        assert_eq!(got, want, "case {name}");
        assert_eq!(got.len(), s.len(), "case {name}: the byte length changed");
    }
}
