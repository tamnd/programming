// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-formatted-output/.

import java.util.Locale;

/** One line of a table with fixed columns. */
public final class Row {
    private Row() {}

    /**
     * Returns name in 10 columns on the left, qty in 5 on the right and price
     * with 2 decimals in 8 on the right. Locale.ROOT keeps the decimal point a
     * dot on each computer. A width is a minimum, so a long value is not cut.
     */
    public static String formatRow(String name, long qty, double price) {
        return String.format(Locale.ROOT, "%-10s|%5d|%8.2f", name, qty, price);
    }

    public static void main(String[] args) {
        System.out.println(formatRow("apple", 3, 1.5));
        System.out.println(formatRow("widget", 12, 19.99));
    }
}
