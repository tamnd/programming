// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wait-for-threads-to-finish/.

//! Sums each chunk on its own scoped thread, waits for all of them and collects the sums.

use std::thread;

/// Returns the sum of each chunk, in the order of chunks.
/// Panics if a thread cannot be created.
pub fn parallel_sums(chunks: &[Vec<i64>]) -> Vec<i64> {
    // A scope joins all its threads before it returns, so the threads may borrow chunks.
    thread::scope(|s| {
        let handles: Vec<_> = chunks
            .iter()
            .map(|chunk| s.spawn(move || chunk.iter().sum::<i64>()))
            .collect();
        handles
            .into_iter()
            .map(|handle| handle.join().expect("a summing thread panicked"))
            .collect()
    })
}
