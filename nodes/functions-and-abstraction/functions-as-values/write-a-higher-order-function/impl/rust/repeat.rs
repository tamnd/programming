// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-higher-order-function/.

/// Returns a function that applies f to its argument times times.
pub fn repeat(f: impl Fn(i32) -> i32, times: i32) -> impl Fn(i32) -> i32 {
    move |x| {
        let mut value = x;
        for _ in 0..times {
            value = f(value);
        }
        value
    }
}

/// Returns n / 2 for an even n and 3n + 1 for an odd n.
pub fn collatz_step(n: i32) -> i32 {
    if n % 2 == 0 { n / 2 } else { 3 * n + 1 }
}

/// Returns the value after times Collatz steps from start.
pub fn collatz_after(start: i32, times: i32) -> i32 {
    repeat(collatz_step, times)(start)
}
