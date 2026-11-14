// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-a-map/.

use std::collections::HashMap;

use algo_iterate_over_a_map::score_lines;

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<(&str, i32)>, Vec<&str>)> = vec![
        ("scores", vec![("bob", 7), ("amy", 9), ("cal", 4)], vec!["amy=9", "bob=7", "cal=4", "total=20"]),
        ("one", vec![("x", 1)], vec!["x=1", "total=1"]),
        ("empty", vec![], vec!["total=0"]),
        ("case", vec![("b", 1), ("B", 2), ("a", 3), ("A", 4)], vec!["A=4", "B=2", "a=3", "b=1", "total=10"]),
        ("negative", vec![("a", -5), ("b", 5)], vec!["a=-5", "b=5", "total=0"]),
        ("prefix", vec![("abc", 3), ("a", 2), ("ab", 1)], vec!["a=2", "ab=1", "abc=3", "total=6"]),
        ("reverse", vec![("j", 1), ("i", 2), ("h", 3), ("g", 4), ("f", 5), ("e", 6), ("d", 7), ("c", 8), ("b", 9), ("a", 10)], vec!["a=10", "b=9", "c=8", "d=7", "e=6", "f=5", "g=4", "h=3", "i=2", "j=1", "total=55"]),
    ];
    for (name, entries, want) in cases {
        let scores: HashMap<&str, i32> = entries.into_iter().collect();
        assert_eq!(score_lines(&scores), want, "{name}");
    }
}
