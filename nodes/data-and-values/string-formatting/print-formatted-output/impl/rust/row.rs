// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-formatted-output/.

/// Returns name in 10 columns on the left, qty in 5 on the right and price
/// with 2 decimals in 8 on the right. A width is a minimum, so a long value
/// is not cut.
pub fn format_row(name: &str, qty: i64, price: f64) -> String {
    format!("{name:<10}|{qty:5}|{price:8.2}")
}
