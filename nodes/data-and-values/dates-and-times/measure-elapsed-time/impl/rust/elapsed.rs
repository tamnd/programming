// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/measure-elapsed-time/.

//! Measures the time that a piece of work takes.

use std::time::{Duration, Instant};

/// Runs `work` once and returns the time it took. `now` is the clock, and
/// `Instant::now` is the real one. An `Instant` is monotonic, and it has a
/// meaning only next to another `Instant`.
pub fn measure(work: impl FnOnce(), mut now: impl FnMut() -> Instant) -> Duration {
    let start = now();
    work();
    now().duration_since(start)
}

/// Returns `end_ns - start_ns` as "M.UUU ms", with the rest under one
/// microsecond dropped.
pub fn elapsed_ms(start_ns: i64, end_ns: i64) -> String {
    let ns = end_ns - start_ns;
    // Integer division truncates, and ns is not negative.
    format!("{}.{:03} ms", ns / 1_000_000, ns / 1_000 % 1_000)
}
