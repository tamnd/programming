// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

use std::collections::HashMap;

use algo_use_a_map_with_a_default_value::lookup;

#[test]
fn cases() {
    type Pairs = Vec<(&'static str, i32)>;
    let cases: Vec<(&str, Pairs, Vec<&str>, i32, Vec<i32>)> = vec![
        ("prices", vec![("apple", 3), ("pear", 5)], vec!["apple", "fig", "pear"], 0, vec![3, 0, 5]),
        ("negative-fallback", vec![("a", 1)], vec!["b", "a"], -1, vec![-1, 1]),
        ("empty-table", vec![], vec!["a", "b"], 7, vec![7, 7]),
        ("empty-keys", vec![("a", 1)], vec![], 0, vec![]),
        ("zero-value", vec![("a", 0)], vec!["a", "b"], 5, vec![0, 5]),
        ("repeat", vec![("a", 1)], vec!["b", "b", "a"], 2, vec![2, 2, 1]),
        ("case", vec![("a", 1)], vec!["A"], 0, vec![0]),
    ];
    for (name, t, keys, fallback, want) in cases {
        let table: HashMap<&str, i32> = t.into_iter().collect();
        let before = table.clone();
        assert_eq!(lookup(&table, &keys, fallback), want, "{name}");
        assert_eq!(table, before, "{name}");
    }
}
