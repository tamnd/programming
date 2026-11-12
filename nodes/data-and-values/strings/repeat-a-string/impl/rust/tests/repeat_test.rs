// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/repeat-a-string/.

use algo_repeat_a_string::repeat;

// The case many has the 200 chars of 100 copies of ab in one literal.
#[test]
fn cases() {
    let cases: [(&str, &str, usize, &str); 9] = [
        ("word", "ab", 3, "ababab"),
        ("one", "abc", 1, "abc"),
        ("dash", "-", 5, "-----"),
        ("zero", "abc", 0, ""),
        ("empty", "", 5, ""),
        ("empty-zero", "", 0, ""),
        ("many", "ab", 100, "abababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababab"),
        ("astral", "\u{1f642}", 3, "\u{1f642}\u{1f642}\u{1f642}"),
        ("combining", "e\u{301}", 2, "e\u{301}e\u{301}"),
    ];
    for (name, s, n, want) in cases {
        assert_eq!(repeat(s, n), want, "{name}");
    }
}
