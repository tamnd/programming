// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-mean-and-median/.

use algo_compute_the_mean_and_median::mean_and_median;

const CASES: &[(&str, &[i64], (f64, f64))] = &[
    ("odd", &[3, 1, 2], (2.0, 2.0)),
    ("even", &[4, 1, 3, 2], (2.5, 2.5)),
    ("skewed", &[1, 2, 3, 4, 100], (22.0, 3.0)),
    ("single", &[5], (5.0, 5.0)),
    ("two-items", &[1, 2], (1.5, 1.5)),
    ("negative", &[-3, -1, -2], (-2.0, -2.0)),
    ("unsorted-even", &[10, 2, 8, 4], (6.0, 6.0)),
    ("duplicates", &[1, 1, 1, 5], (2.0, 1.0)),
    ("not-an-integer", &[1, 2, 2], (1.6666666666666667, 2.0)),
    ("large", &[9223372036854775807, 9223372036854775807], (9223372036854775808.0, 9223372036854775808.0)),
];

fn close(got: f64, want: f64) -> bool {
    (got - want).abs() <= f64::max(1e-9, 1e-12 * want.abs())
}

#[test]
fn cases() {
    for &(name, items, (mean, median)) in CASES {
        let (gm, gd) = mean_and_median(items);
        assert!(close(gm, mean) && close(gd, median), "{name}: got ({gm}, {gd})");
    }
}

#[test]
#[should_panic(expected = "the list is empty")]
fn empty_panics() {
    mean_and_median(&[]);
}
