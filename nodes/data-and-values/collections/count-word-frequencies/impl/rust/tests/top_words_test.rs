// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-word-frequencies/.

use algo_count_word_frequencies::top_words;

type Case = (&'static str, &'static str, usize, &'static [(&'static str, u64)]);

const CASES: &[Case] = &[
    ("sentence", "the cat and the hat and the bat", 3, &[("the", 3), ("and", 2), ("bat", 1)]),
    ("all-words", "one two two three three three", 10, &[("three", 3), ("two", 2), ("one", 1)]),
    ("punctuation", "Stop. Stop, stop! Go?", 2, &[("stop", 3), ("go", 1)]),
    ("lines", "red green\nblue red\ngreen red\n", 2, &[("red", 3), ("green", 2)]),
    ("empty", "", 3, &[]),
    ("no-words", " ... ,!? \n", 3, &[]),
    ("case", "Word word WORD wOrD", 1, &[("word", 4)]),
    ("ties", "pear fig apple fig pear apple", 3, &[("apple", 2), ("fig", 2), ("pear", 2)]),
    ("fold-before-sort", "Zebra apple", 2, &[("apple", 1), ("zebra", 1)]),
    ("n-zero", "a b c", 0, &[]),
    ("n-above-count", "a b a", 10, &[("a", 2), ("b", 1)]),
    ("non-ascii", "café naïve café 日本語 naïve café", 3, &[("café", 3), ("naïve", 2), ("日本語", 1)]),
    ("code-point-order", "f é e", 3, &[("e", 1), ("f", 1), ("é", 1)]),
    ("apostrophe", "don't stop, don't", 3, &[("don", 2), ("t", 2), ("stop", 1)]),
    ("digits", "route 66 and route 66a 66", 3, &[("66", 2), ("route", 2), ("66a", 1)]),
    ("separators", "tab\there\r\nunder_score-dash", 10, &[("dash", 1), ("here", 1), ("score", 1), ("tab", 1), ("under", 1)]),
];

#[test]
fn cases() {
    for &(name, text, n, want) in CASES {
        let want: Vec<(String, u64)> = want.iter().map(|&(w, c)| (w.to_string(), c)).collect();
        assert_eq!(top_words(text, n), want, "{name}");
    }
}
