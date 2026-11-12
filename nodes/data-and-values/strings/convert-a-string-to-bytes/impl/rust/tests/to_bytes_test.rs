// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-a-string-to-bytes/.

use algo_convert_a_string_to_bytes::to_bytes;

// The encoding is UTF-8. U+00E9 is 2 bytes, U+20AC is 3 bytes and U+1F642 is 4 bytes. In Latin-1, U+00FF would be 1 byte.
#[test]
fn cases() {
    let cases: [(&str, &str, &[u8]); 12] = [
        ("ascii", "abc", &[97, 98, 99]),
        ("letter", "A", &[65]),
        ("punct", "hi!", &[104, 105, 33]),
        ("empty", "", &[]),
        ("control", "\t\n", &[9, 10]),
        ("two-byte", "\u{e9}", &[195, 169]),
        ("latin-1-max", "\u{ff}", &[195, 191]),
        ("three-byte", "\u{20ac}", &[226, 130, 172]),
        ("bom", "\u{feff}", &[239, 187, 191]),
        ("astral", "\u{1f642}", &[240, 159, 153, 130]),
        ("combining", "e\u{301}", &[101, 204, 129]),
        ("mixed", "a\u{e9}\u{1f642}", &[97, 195, 169, 240, 159, 153, 130]),
    ];
    for (name, s, want) in cases {
        assert_eq!(to_bytes(s), want, "{name}");
    }
}
