// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-edit-distance-of-two-strings/.

use algo_compute_the_edit_distance_of_two_strings::levenshtein;

const CASES: &[(&str, &str, &str, usize)] = &[
    ("kitten-sitting", "kitten", "sitting", 3),
    ("rosettacode", "rosettacode", "raisethysword", 8),
    ("flaw-lawn", "flaw", "lawn", 2),
    ("both-empty", "", "", 0),
    ("empty-left", "", "abc", 3),
    ("empty-right", "abc", "", 3),
    ("equal", "same", "same", 0),
    ("one-substitution", "a", "b", 1),
    ("transposition", "ab", "ba", 2),
    ("case", "Apple", "apple", 1),
    ("disjoint", "abc", "xyz", 3),
    ("prefix", "abc", "abcdef", 3),
    ("intention", "intention", "execution", 5),
    ("space", "a b", "ab", 1),
];

#[test]
fn cases() {
    for &(name, a, b, want) in CASES {
        assert_eq!(levenshtein(a, b), want, "{name}");
        assert_eq!(levenshtein(b, a), want, "{name} swapped");
    }
}
