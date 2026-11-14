// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-map/.

use algo_create_a_map::build_map;

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<(&str, i32)>, Vec<&str>, Vec<(&str, i32)>)> = vec![
        ("fruits", vec![("apple", 3), ("pear", 5), ("fig", 2)], vec![], vec![("apple", 3), ("fig", 2), ("pear", 5)]),
        ("update-and-delete", vec![("a", 1), ("b", 2), ("a", 7), ("c", 3)], vec!["b"], vec![("a", 7), ("c", 3)]),
        ("empty", vec![], vec![], vec![]),
        ("replace", vec![("k", 1), ("k", 2), ("k", 3)], vec![], vec![("k", 3)]),
        ("delete-absent", vec![("a", 1)], vec!["z"], vec![("a", 1)]),
        ("delete-all", vec![("a", 1), ("b", 2)], vec!["a", "b"], vec![]),
        ("delete-twice", vec![("a", 1), ("b", 2)], vec!["a", "a"], vec![("b", 2)]),
        ("zero-value", vec![("a", 0), ("b", -4)], vec![], vec![("a", 0), ("b", -4)]),
        ("case", vec![("b", 1), ("B", 2), ("a", 3), ("A", 4)], vec![], vec![("A", 4), ("B", 2), ("a", 3), ("b", 1)]),
    ];
    for (name, pairs, remove, want) in cases {
        assert_eq!(build_map(&pairs, &remove), want, "{name}");
    }
}
