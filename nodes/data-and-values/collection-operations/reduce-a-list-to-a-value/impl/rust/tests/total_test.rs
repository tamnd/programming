// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

use algo_reduce_a_list_to_a_value::total;

#[test]
fn cases() {
    let cases: &[(&str, &[i32], i32)] = &[
        ("small", &[1, 2, 3, 4], 10),
        ("mixed", &[10, -3, 5], 12),
        ("empty", &[], 0),
        ("single", &[7], 7),
        ("negative", &[-1, -2, -3], -6),
        ("zero-sum", &[5, -5], 0),
        ("max", &[2147483647], 2147483647),
        ("min", &[-2147483648], -2147483648),
        ("to-the-limit", &[2147483646, 1], 2147483647),
        ("partial-overflow", &[2147483647, 1, -1], 2147483647),
        ("partial-underflow", &[-2147483648, -1, 1], -2147483648),
    ];
    for &(name, items, want) in cases {
        assert_eq!(total(items), want, "{name}");
    }
}
