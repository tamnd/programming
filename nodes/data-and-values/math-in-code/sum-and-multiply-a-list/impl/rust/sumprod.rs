// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sum-and-multiply-a-list/.

/// Returns the sum and the product of the items. With overflow checks on,
/// sum and product panic on overflow.
pub fn sum_and_product(items: &[i64]) -> (i64, i64) {
    (items.iter().sum(), items.iter().product())
}

/// Returns None if a partial sum or product overflows.
pub fn checked_sum_and_product(items: &[i64]) -> Option<(i64, i64)> {
    let sum = items.iter().try_fold(0i64, |a, &x| a.checked_add(x))?;
    let product = items.iter().try_fold(1i64, |a, &x| a.checked_mul(x))?;
    Some((sum, product))
}
