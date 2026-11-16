// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-lazy-sequences/.

use algo_use_lazy_sequences::{first_primes, primes};

#[test]
fn cases() {
    let cases: Vec<(&str, usize, Vec<u32>)> = vec![
        ("five", 5, vec![2, 3, 5, 7, 11]),
        ("ten", 10, vec![2, 3, 5, 7, 11, 13, 17, 19, 23, 29]),
        ("twenty-five", 25, vec![2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97]),
        ("zero", 0, vec![]),
        ("one", 1, vec![2]),
        ("two", 2, vec![2, 3]),
        ("past-a-gap", 31, vec![2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127]),
    ];
    for (name, n, want) in cases {
        assert_eq!(first_primes(n), want, "{name}");
    }
}

#[test]
fn primes_is_lazy() {
    assert_eq!(primes().nth(3), Some(7));
}
