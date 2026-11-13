// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-graphemes-of-a-string/.

use algo_count_the_graphemes_of_a_string::count_graphemes;

// A flag is 2 regional indicators, and U+1F469 U+200D U+1F4BB is 3 code points. Each of them is one grapheme.
#[test]
fn cases() {
    let cases: [(&str, &str, usize); 18] = [
        ("empty", "", 0),
        ("ascii", "abc", 3),
        ("precomposed", "\u{e9}", 1),
        ("emoji", "\u{1f642}", 1),
        ("combining", "e\u{301}", 1),
        ("two-marks", "e\u{301}\u{302}", 1),
        ("leading-mark", "\u{301}a", 2),
        ("crlf", "\r\n", 1),
        ("lfcr", "\n\r", 2),
        ("cr-mark", "\r\u{301}", 2),
        ("soft-hyphen", "a\u{ad}b", 3),
        ("flag", "\u{1f1eb}\u{1f1f7}", 1),
        ("three-indicators", "\u{1f1eb}\u{1f1f7}\u{1f1ea}", 2),
        ("two-flags", "\u{1f1eb}\u{1f1f7}\u{1f1ea}\u{1f1f8}", 2),
        ("zwj-sequence", "\u{1f469}\u{200d}\u{1f4bb}", 1),
        ("zwj-after-letter", "a\u{200d}\u{1f642}", 2),
        ("skin-tone", "\u{1f44d}\u{1f3fd}", 1),
        ("variation-selector", "\u{2764}\u{fe0f}", 1),
    ];
    for (name, s, want) in cases {
        assert_eq!(count_graphemes(s), want, "{name}");
    }
}
