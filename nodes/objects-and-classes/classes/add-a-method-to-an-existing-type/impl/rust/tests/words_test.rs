// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.

use algo_add_a_method_to_an_existing_type::{word_count, WordCount};

#[test]
fn cases() {
    let cases: Vec<(&str, &str, usize)> = vec![
        ("three-words", "the quick fox", 3),
        ("one-word", "hello", 1),
        ("punctuation", "Hello, world! How are you?", 5),
        ("empty", "", 0),
        ("blank", " \t\n ", 0),
        ("mixed-whitespace", "  two\twords\r\nand three  ", 4),
        ("non-ascii", "naïve café 東京", 3),
    ];
    for (name, text, want) in cases {
        assert_eq!(word_count(text), want, "{name}");
    }
}

#[test]
fn method_works_on_str_and_string() {
    assert_eq!("one two".word_count(), 2);
    assert_eq!(String::from("a b c").word_count(), 3);
}
