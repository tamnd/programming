// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-set-operations/.

use algo_do_set_operations::{compare_sets, SetOps};

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<&str>, Vec<&str>, SetOps)> = vec![
        (
            "colors",
            vec!["red", "blue", "green"],
            vec!["blue", "yellow"],
            SetOps { union: vec!["blue", "green", "red", "yellow"], intersection: vec!["blue"], difference: vec!["green", "red"], symmetric_difference: vec!["green", "red", "yellow"], is_subset: false },
        ),
        (
            "subset",
            vec!["b"],
            vec!["a", "b", "c"],
            SetOps { union: vec!["a", "b", "c"], intersection: vec!["b"], difference: vec![], symmetric_difference: vec!["a", "c"], is_subset: true },
        ),
        (
            "both-empty",
            vec![],
            vec![],
            SetOps { union: vec![], intersection: vec![], difference: vec![], symmetric_difference: vec![], is_subset: true },
        ),
        (
            "empty-a",
            vec![],
            vec!["x"],
            SetOps { union: vec!["x"], intersection: vec![], difference: vec![], symmetric_difference: vec!["x"], is_subset: true },
        ),
        (
            "empty-b",
            vec!["x"],
            vec![],
            SetOps { union: vec!["x"], intersection: vec![], difference: vec!["x"], symmetric_difference: vec!["x"], is_subset: false },
        ),
        (
            "equal",
            vec!["a", "b"],
            vec!["b", "a"],
            SetOps { union: vec!["a", "b"], intersection: vec!["a", "b"], difference: vec![], symmetric_difference: vec![], is_subset: true },
        ),
        (
            "disjoint",
            vec!["a"],
            vec!["b"],
            SetOps { union: vec!["a", "b"], intersection: vec![], difference: vec!["a"], symmetric_difference: vec!["a", "b"], is_subset: false },
        ),
        (
            "duplicates",
            vec!["a", "a", "b"],
            vec!["b", "b"],
            SetOps { union: vec!["a", "b"], intersection: vec!["b"], difference: vec!["a"], symmetric_difference: vec!["a"], is_subset: false },
        ),
        (
            "case",
            vec!["a"],
            vec!["A"],
            SetOps { union: vec!["A", "a"], intersection: vec![], difference: vec!["a"], symmetric_difference: vec!["A", "a"], is_subset: false },
        ),
    ];
    for (name, a, b, want) in cases {
        assert_eq!(compare_sets(&a, &b), want, "{name}");
    }
}
