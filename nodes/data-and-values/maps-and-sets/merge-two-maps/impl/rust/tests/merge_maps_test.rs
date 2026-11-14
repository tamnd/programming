// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/merge-two-maps/.

use std::collections::HashMap;

use algo_merge_two_maps::merge_maps;

#[test]
fn cases() {
    type Pairs = Vec<(&'static str, i32)>;
    let cases: Vec<(&str, Pairs, Pairs, Pairs)> = vec![
        ("prices", vec![("apple", 3), ("pear", 5)], vec![("pear", 6), ("fig", 2)], vec![("apple", 3), ("fig", 2), ("pear", 6)]),
        ("disjoint", vec![("a", 1)], vec![("b", 2)], vec![("a", 1), ("b", 2)]),
        ("empty-base", vec![], vec![("a", 1)], vec![("a", 1)]),
        ("empty-override", vec![("a", 1)], vec![], vec![("a", 1)]),
        ("both-empty", vec![], vec![], vec![]),
        ("same-keys", vec![("a", 1), ("b", 2)], vec![("a", 9), ("b", 8)], vec![("a", 9), ("b", 8)]),
        ("zero-wins", vec![("a", 5)], vec![("a", 0)], vec![("a", 0)]),
        ("case", vec![("a", 1)], vec![("A", 2)], vec![("A", 2), ("a", 1)]),
    ];
    for (name, b, o, want) in cases {
        let base: HashMap<&str, i32> = b.into_iter().collect();
        let over: HashMap<&str, i32> = o.into_iter().collect();
        let (base_before, over_before) = (base.clone(), over.clone());
        assert_eq!(merge_maps(&base, &over), want, "{name}");
        assert_eq!(base, base_before, "{name}");
        assert_eq!(over, over_before, "{name}");
    }
}
