// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-number-is-prime/.

use algo_check_whether_a_number_is_prime::is_prime;

const CASES: &[(&str, i64, bool)] = &[
    ("small-prime", 17, true),
    ("small-composite", 15, false),
    ("even", 100, false),
    ("odd-composite", 91, false),
    ("negative", -7, false),
    ("zero", 0, false),
    ("one", 1, false),
    ("two", 2, true),
    ("three", 3, true),
    ("square-of-prime", 25, false),
    ("large-square", 4293001441, false),
    ("carmichael", 561, false),
    ("large-prime", 1000000007, true),
    ("mersenne-prime", 2147483647, true),
    ("large-composite", 2147483649, false),
];

#[test]
fn cases() {
    for &(name, n, want) in CASES {
        assert_eq!(is_prime(n), want, "{name}");
    }
}

#[test]
fn primes_below_30() {
    let primes: Vec<i64> = (0..30).filter(|&n| is_prime(n)).collect();
    assert_eq!(primes, [2, 3, 5, 7, 11, 13, 17, 19, 23, 29]);
}
