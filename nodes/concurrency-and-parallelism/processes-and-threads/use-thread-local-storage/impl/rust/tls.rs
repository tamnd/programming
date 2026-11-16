// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-thread-local-storage/.

//! Counts on a thread-local counter in one thread for each count, with no lock and no shared counter.

use std::cell::Cell;
use std::thread;

thread_local! {
    // Each thread gets its own Cell, which starts at 0. A const initializer needs no lazy setup.
    static COUNTER: Cell<u32> = const { Cell::new(0) };
}

/// Adds 1 to the counter of the current thread and returns the new value.
pub fn bump() -> u32 {
    COUNTER.with(|c| {
        c.set(c.get() + 1);
        c.get()
    })
}

/// Returns the counter of the current thread.
pub fn current() -> u32 {
    COUNTER.with(Cell::get)
}

/// Starts one thread for each count and returns the final counter of each thread, in order.
/// Panics if a thread cannot be created.
pub fn count_per_thread(counts: &[u32]) -> Vec<u32> {
    // The scope joins every thread before it returns, so the threads can borrow counts.
    thread::scope(|s| {
        let handles: Vec<_> = counts
            .iter()
            .map(|&n| {
                s.spawn(move || {
                    for _ in 0..n {
                        bump();
                    }
                    current()
                })
            })
            .collect();
        handles
            .into_iter()
            .map(|h| h.join().expect("no thread panicked"))
            .collect()
    })
}
