// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sleep-for-a-duration/.

//! Sleeps for a duration on a monotonic clock.

use std::time::{Duration, Instant};

/// Sleeps until `clock` reaches the deadline. A sleep that ends early is
/// followed by a sleep for the rest of the time.
pub fn sleep_loop(duration_ns: i64, mut clock: impl FnMut() -> i64, mut sleep: impl FnMut(i64)) {
    let mut now = clock();
    let deadline = now + duration_ns;
    while now < deadline {
        sleep(deadline - now);
        now = clock();
    }
}

/// Blocks the thread for at least `duration_ns` nanoseconds. `Instant` is
/// monotonic.
pub fn sleep_for(duration_ns: i64) {
    let base = Instant::now();
    let clock = || i64::try_from(base.elapsed().as_nanos()).unwrap_or(i64::MAX);
    sleep_loop(duration_ns, clock, |ns| {
        std::thread::sleep(Duration::from_nanos(ns.unsigned_abs()));
    });
}

/// Runs `sleep_loop` on a fake clock and returns the durations it asks for.
#[must_use]
pub fn sleep_requests(duration_ns: i64, start_ns: i64, readings: &[i64]) -> Vec<i64> {
    let mut times = std::iter::once(start_ns).chain(readings.iter().copied());
    let mut requests = Vec::new();
    sleep_loop(
        duration_ns,
        || times.next().expect("a reading after each sleep"),
        |ns| requests.push(ns),
    );
    requests
}
