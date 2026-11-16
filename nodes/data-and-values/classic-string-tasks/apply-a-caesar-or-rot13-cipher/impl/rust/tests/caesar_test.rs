// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-caesar-or-rot13-cipher/.

use algo_apply_a_caesar_or_rot13_cipher::caesar;

const CASES: &[(&str, &str, i64, &str)] = &[
    ("hi-2", "HI", 2, "JK"),
    ("hi-20", "HI", 20, "BC"),
    ("rot13", "Hello, World!", 13, "Uryyb, Jbeyq!"),
    ("empty", "", 3, ""),
    ("zero", "abc", 0, "abc"),
    ("full-turn", "abc", 26, "abc"),
    ("minus-one", "abc", -1, "zab"),
    ("decode", "Uryyb, Jbeyq!", -13, "Hello, World!"),
    ("above-26", "xyz", 27, "yza"),
    ("wrap", "Zz", 1, "Aa"),
    ("non-letters", "123 !? [`{@", 5, "123 !? [`{@"),
    ("non-ascii", "café ü", 1, "dbgé ü"),
    ("large", "abc", 9007199254740991, "fgh"),
    ("large-negative", "abc", -9007199254740991, "vwx"),
];

#[test]
fn cases() {
    for &(name, text, shift, want) in CASES {
        assert_eq!(caesar(text, shift), want, "{name}");
        assert_eq!(caesar(want, -shift), text, "{name} decoded");
    }
}
