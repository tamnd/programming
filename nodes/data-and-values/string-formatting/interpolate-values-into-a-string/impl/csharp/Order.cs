// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/interpolate-values-into-a-string/.

using System.Globalization;

/// <summary>A sentence built from the values of an order.</summary>
public static class Order
{
    /// <summary>
    /// Returns "Order {name}: count item(s), total count*price". The doubled
    /// braces are literal braces, the conditional is in parentheses, and the
    /// invariant culture keeps the decimal point a dot.
    /// </summary>
    public static string DescribeOrder(string name, long count, double price) =>
        string.Create(
            CultureInfo.InvariantCulture,
            $"Order {{{name}}}: {count} item{(count == 1 ? "" : "s")}, total {count * price:F2}");
}
