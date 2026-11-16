// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-tasks-on-a-thread-pool/.

use algo_run_tasks_on_a_thread_pool::{collatz_steps, steps_on_pool};

#[test]
fn cases() {
    let many: Vec<u32> = (1..=200).collect();
    let many_want: Vec<u32> = many.iter().map(|&n| collatz_steps(u64::from(n))).collect();
    let cases: Vec<(&str, usize, Vec<u32>, Vec<u32>)> = vec![
        ("four-jobs", 2, vec![1, 2, 3, 6], vec![0, 1, 7, 8]),
        ("long-runs", 3, vec![27, 97, 871], vec![111, 118, 178]),
        ("empty", 2, vec![], vec![]),
        ("one-worker", 1, vec![7, 8, 9], vec![16, 3, 19]),
        ("more-workers-than-jobs", 8, vec![5, 6], vec![5, 8]),
        ("duplicates", 3, vec![27, 27, 27, 1], vec![111, 111, 111, 0]),
        ("wide-intermediate", 2, vec![113383, 837799], vec![247, 524]),
        ("many", 4, many, many_want),
    ];
    for (name, workers, numbers, want) in cases {
        assert_eq!(steps_on_pool(workers, &numbers), want, "case {name}");
    }
}
