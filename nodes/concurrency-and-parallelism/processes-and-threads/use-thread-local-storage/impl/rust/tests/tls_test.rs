// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-thread-local-storage/.

use algo_use_thread_local_storage::{bump, count_per_thread, current};
use std::thread;

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<u32>)> = vec![
        ("three-threads", vec![1, 2, 3]),
        ("uneven", vec![10, 0, 5, 1000]),
        ("empty", vec![]),
        ("zero", vec![0]),
        ("one-thread", vec![7]),
        ("equal-counts", vec![100, 100, 100, 100]),
        ("many-threads", (1..=16).collect()),
        ("large", vec![100000, 100000]),
    ];
    for (name, counts) in cases {
        assert_eq!(count_per_thread(&counts), counts, "case {name}");
    }
}

#[test]
fn caller_counter_does_not_change() {
    // Run in a new thread, so that other tests on this thread do not matter.
    let seen = thread::spawn(|| {
        for _ in 0..3 {
            bump();
        }
        count_per_thread(&[5, 6]);
        current()
    })
    .join()
    .expect("the caller did not panic");
    assert_eq!(seen, 3);
}
