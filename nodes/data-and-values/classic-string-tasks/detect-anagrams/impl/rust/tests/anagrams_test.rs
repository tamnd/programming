// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-anagrams/.

use algo_detect_anagrams::are_anagrams;

const CASES: &[(&str, &str, &str, bool)] = &[
    ("listen-silent", "listen", "silent", true),
    ("dormitory", "Dormitory", "dirty room", true),
    ("hello-world", "hello", "world", false),
    ("both-empty", "", "", true),
    ("one-empty", "a", "", false),
    ("same-set-other-counts", "aab", "abb", false),
    ("extra-letter", "abc", "abcd", false),
    ("same-length-other-letters", "abc", "abd", false),
    ("case", "Listen", "SILENT", true),
    ("same-word", "stop", "stop", true),
    ("gentleman", "a gentleman", "elegant man", true),
    ("they-see", "The eyes!", "They see.", true),
    ("punctuation-only", "!?", "...", true),
];

#[test]
fn cases() {
    for &(name, a, b, want) in CASES {
        assert_eq!(are_anagrams(a, b), want, "{name}");
        assert_eq!(are_anagrams(b, a), want, "{name} swapped");
    }
}
