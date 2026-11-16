// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-floating-point-numbers/.

/// Tells whether `a` and `b` are close:
/// |a - b| <= max(rel_tol * max(|a|, |b|), abs_tol).
/// NaN is close to no value, and an infinity is close only to itself.
pub fn is_close(a: f64, b: f64, rel_tol: f64, abs_tol: f64) -> bool {
    if a == b {
        return true; // also an infinity and itself
    }
    if !a.is_finite() || !b.is_finite() {
        return false; // NaN or an infinity
    }
    let diff = (a - b).abs();
    diff <= (rel_tol * a.abs().max(b.abs())).max(abs_tol)
}
