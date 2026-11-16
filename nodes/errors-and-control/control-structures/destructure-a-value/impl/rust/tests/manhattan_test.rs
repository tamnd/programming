// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/destructure-a-value/.

use algo_destructure_a_value::manhattan;

#[test]
fn cases() {
    let cases: &[(&str, [i32; 2], [i32; 2], i32)] = &[
        ("first", [1, 2], [4, 6], 7),
        ("on-a-line", [0, 0], [10, 0], 10),
        ("down-left", [5, 5], [2, 1], 7),
        ("mixed-signs", [3, -1], [-2, 4], 10),
        ("same-point", [7, 7], [7, 7], 0),
        ("negative", [-3, -4], [-1, -1], 5),
        ("large", [1000000, -1000000], [-1000000, 1000000], 4000000),
    ];
    for &(name, p, q, want) in cases {
        assert_eq!(manhattan(p, q), want, "{name}");
    }
}
