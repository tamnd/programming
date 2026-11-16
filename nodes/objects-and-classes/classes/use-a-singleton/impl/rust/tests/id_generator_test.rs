// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-singleton/.

use algo_use_a_singleton::{singleton_report, IdGenerator};
use std::thread;

#[test]
fn cases() {
    let cases: Vec<(&str, u32, Vec<u64>)> = vec![
        ("three-calls", 3, vec![2, 1, 1]),
        ("ten-calls", 10, vec![9, 1, 1]),
        ("single-call", 1, vec![0, 1, 1]),
        ("pair", 2, vec![1, 1, 1]),
        ("limit", 1000000, vec![999999, 1, 1]),
    ];
    for (name, calls, want) in cases {
        assert_eq!(singleton_report(calls), want, "{name}");
    }
}

#[test]
fn threads_share_one_instance() {
    let handles: Vec<_> = (0..8)
        .map(|_| thread::spawn(|| IdGenerator::instance() as *const IdGenerator as usize))
        .collect();
    let first = IdGenerator::instance() as *const IdGenerator as usize;
    for h in handles {
        assert_eq!(h.join().unwrap(), first);
    }
    assert_eq!(IdGenerator::constructions(), 1);
}
