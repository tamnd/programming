// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/interpolate-values-into-a-string/.

/// Returns "Order {name}: count item(s), total count*price". A name in
/// braces must be a variable, so the ending and the total come first.
pub fn describe_order(name: &str, count: i64, price: f64) -> String {
    let ending = if count == 1 { "" } else { "s" };
    let total = count as f64 * price;
    format!("Order {{{name}}}: {count} item{ending}, total {total:.2}")
}
