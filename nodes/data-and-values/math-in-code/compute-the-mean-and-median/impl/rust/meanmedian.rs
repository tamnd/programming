// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-mean-and-median/.

/// Returns the mean and the median of a non-empty slice. The sum is an
/// f64, so it does not overflow. The function sorts a copy.
pub fn mean_and_median(items: &[i64]) -> (f64, f64) {
    assert!(!items.is_empty(), "the list is empty");
    let n = items.len();
    let mean = items.iter().map(|&x| x as f64).sum::<f64>() / n as f64;
    let mut s = items.to_vec();
    s.sort_unstable();
    let mid = n / 2;
    let median = if n % 2 == 1 {
        s[mid] as f64
    } else {
        (s[mid - 1] as f64 + s[mid] as f64) / 2.0
    };
    (mean, median)
}
