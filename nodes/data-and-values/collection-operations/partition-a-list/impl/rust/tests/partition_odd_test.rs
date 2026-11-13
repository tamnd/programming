// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/partition-a-list/.

use algo_partition_a_list::partition_odd;

#[test]
fn cases() {
    let cases: Vec<(&str, &[i32], Vec<i32>, Vec<i32>)> = vec![
        ("mixed", &[1, 2, 3, 4, 5], vec![1, 3, 5], vec![2, 4]),
        ("tens", &[10, 15, 20, 25], vec![15, 25], vec![10, 20]),
        ("empty", &[], vec![], vec![]),
        ("all-odd", &[1, 3], vec![1, 3], vec![]),
        ("all-even", &[2, 4], vec![], vec![2, 4]),
        ("negative", &[-3, -2, -1, 0], vec![-3, -1], vec![-2, 0]),
        ("zero", &[0], vec![], vec![0]),
        ("order", &[9, 8, 1, 2], vec![9, 1], vec![8, 2]),
        ("extremes", &[-2147483648, 2147483647], vec![2147483647], vec![-2147483648]),
        ("duplicates", &[3, 3, 2, 2], vec![3, 3], vec![2, 2]),
    ];
    for (name, items, odd, even) in cases {
        assert_eq!(partition_odd(items), (odd, even), "{name}");
    }
}
