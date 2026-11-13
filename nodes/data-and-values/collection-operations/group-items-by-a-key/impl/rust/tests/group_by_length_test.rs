// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/group-items-by-a-key/.

use algo_group_items_by_a_key::group_by_length;

#[test]
fn cases() {
    let cases: Vec<(&str, &[&str], Vec<(usize, Vec<&str>)>)> = vec![
        ("fruits", &["apple", "fig", "kiwi", "pear", "plum", "date"], vec![(3, vec!["fig"]), (4, vec!["kiwi", "pear", "plum", "date"]), (5, vec!["apple"])]),
        ("letters", &["a", "bb", "c"], vec![(1, vec!["a", "c"]), (2, vec!["bb"])]),
        ("empty", &[], vec![]),
        ("single", &["x"], vec![(1, vec!["x"])]),
        ("same-length", &["cat", "dog", "cow"], vec![(3, vec!["cat", "dog", "cow"])]),
        ("order", &["ccc", "a", "bb"], vec![(1, vec!["a"]), (2, vec!["bb"]), (3, vec!["ccc"])]),
        ("duplicates", &["ab", "ab", "c"], vec![(1, vec!["c"]), (2, vec!["ab", "ab"])]),
        ("long", &["abcdefghij", "abcdefghi", "a"], vec![(1, vec!["a"]), (9, vec!["abcdefghi"]), (10, vec!["abcdefghij"])]),
    ];
    for (name, words, want) in cases {
        assert_eq!(group_by_length(words), want, "{name}");
    }
}
