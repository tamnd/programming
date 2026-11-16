// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-a-palindrome/.

use algo_check_whether_a_string_is_a_palindrome::is_palindrome;

const CASES: &[(&str, &str, bool)] = &[
    ("racecar", "racecar", true),
    ("panama", "A man, a plan, a canal: Panama", true),
    ("hello", "hello", false),
    ("empty", "", true),
    ("only-punctuation", "?! ...", true),
    ("one-letter", "a", true),
    ("two-letters", "ab", false),
    ("mixed-case", "Aa", true),
    ("nixon", "No 'x' in Nixon", true),
    ("cat-i-saw", "Was it a car or a cat I saw?", true),
    ("digits", "12321", true),
    ("digits-not", "123", false),
    ("zero-p", "0P", false),
    ("race-a-car", "race a car", false),
    ("even-near", "abca", false),
];

#[test]
fn cases() {
    for &(name, text, want) in CASES {
        assert_eq!(is_palindrome(text), want, "{name}");
    }
}
