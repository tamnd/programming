// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-queue/.

use std::collections::VecDeque;

/// Returns the job numbers in the order in which round-robin finishes them.
pub fn finish_order(work: &[u32], slice: u32) -> Vec<usize> {
    let mut left = work.to_vec();
    let mut queue: VecDeque<usize> = (0..work.len()).collect();
    let mut done = Vec::new();
    while let Some(job) = queue.pop_front() {
        left[job] -= slice.min(left[job]);
        if left[job] == 0 {
            done.push(job);
        } else {
            queue.push_back(job);
        }
    }
    done
}
