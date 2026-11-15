// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-a-step/.

/// Returns start, start + step, ... while the value has not passed stop.
/// step must not be 0.
pub fn count_by(start: i32, stop: i32, step: i32) -> Vec<i32> {
    let mut out = Vec::new();
    // step_by takes a usize, so the sign of the step picks the direction.
    let k = step.unsigned_abs() as usize;
    if step > 0 {
        for i in (start..=stop).step_by(k) {
            out.push(i);
        }
    } else {
        // start..=stop is empty when start > stop, so reverse stop..=start.
        for i in (stop..=start).rev().step_by(k) {
            out.push(i);
        }
    }
    out
}
