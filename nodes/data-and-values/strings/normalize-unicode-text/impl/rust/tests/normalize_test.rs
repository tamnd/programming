// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/normalize-unicode-text/.

use algo_normalize_unicode_text::normalize;

// U+00E9 is NFC, and e U+0301 is NFD. Both are the same text.
#[test]
fn cases() {
    let cases: [(&str, &str, &str, &str); 15] = [
        ("nfc-composes", "e\u{301}", "NFC", "\u{e9}"),
        ("nfd-decomposes", "\u{e9}", "NFD", "e\u{301}"),
        ("ascii", "abc", "NFC", "abc"),
        ("empty", "", "NFD", ""),
        ("already-nfc", "\u{e9}", "NFC", "\u{e9}"),
        ("already-nfd", "e\u{301}", "NFD", "e\u{301}"),
        ("mixed-nfc", "cafe\u{301} caf\u{e9}", "NFC", "caf\u{e9} caf\u{e9}"),
        ("mixed-nfd", "cafe\u{301} caf\u{e9}", "NFD", "cafe\u{301} cafe\u{301}"),
        ("upper-grave", "E\u{300}", "NFC", "\u{c8}"),
        ("diaeresis", "u\u{308}ber", "NFC", "\u{fc}ber"),
        ("circumflex", "\u{ee}", "NFD", "i\u{302}"),
        ("umlauts", "\u{c4}\u{d6}\u{dc}\u{e4}\u{f6}\u{fc}", "NFD", "A\u{308}O\u{308}U\u{308}a\u{308}o\u{308}u\u{308}"),
        ("spacing-accent", "\u{b4}e", "NFC", "\u{b4}e"),
        ("no-break-space", "a\u{a0}b", "NFD", "a\u{a0}b"),
        ("sharp-s", "stra\u{df}e", "NFD", "stra\u{df}e"),
    ];
    for (name, s, form, want) in cases {
        assert_eq!(normalize(s, form), want, "{name}");
    }
}
