// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-tasks-on-a-thread-pool/.

//! Sends one job for each number to a fixed set of worker threads through a shared queue,
//! and collects the results in order.

use std::sync::{mpsc, Mutex};
use std::thread;

/// Returns the number of steps that take n to 1.
pub fn collatz_steps(mut n: u64) -> u32 {
    let mut steps = 0;
    while n != 1 {
        n = if n % 2 == 0 { n / 2 } else { 3 * n + 1 };
        steps += 1;
    }
    steps
}

/// Returns the step count of each number, in the order of numbers.
/// Panics if a worker thread cannot be created.
pub fn steps_on_pool(workers: usize, numbers: &[u32]) -> Vec<u32> {
    let (job_tx, job_rx) = mpsc::channel::<usize>();
    let (result_tx, result_rx) = mpsc::channel::<(usize, u32)>();
    for i in 0..numbers.len() {
        job_tx.send(i).expect("the queue is open");
    }
    // With the sender gone, recv returns an error when the queue is empty, and the workers stop.
    drop(job_tx);
    // Several workers share one receiver, so it is behind a mutex.
    let job_rx = Mutex::new(job_rx);
    // The scope joins every worker before it returns.
    thread::scope(|s| {
        for _ in 0..workers {
            let job_rx = &job_rx;
            let result_tx = result_tx.clone();
            s.spawn(move || loop {
                // The lock guard is a temporary of this let statement, so the lock is free again
                // before the job runs.
                let job = job_rx.lock().expect("no worker panicked").recv();
                match job {
                    Ok(i) => {
                        let steps = collatz_steps(u64::from(numbers[i]));
                        result_tx.send((i, steps)).expect("the result queue is open");
                    }
                    Err(_) => break,
                }
            });
        }
    });
    drop(result_tx);
    let mut results = vec![0; numbers.len()];
    for (i, steps) in result_rx {
        results[i] = steps;
    }
    results
}
