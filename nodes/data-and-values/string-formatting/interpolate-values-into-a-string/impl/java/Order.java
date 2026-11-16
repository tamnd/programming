// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/interpolate-values-into-a-string/.

import java.util.Locale;

/** A sentence built from the values of an order. */
public final class Order {
    private Order() {}

    /**
     * Returns "Order {name}: count item(s), total count*price". Java has no
     * string interpolation, so this uses String.format. Locale.ROOT keeps the
     * decimal point a dot.
     */
    public static String describeOrder(String name, long count, double price) {
        String ending = count == 1 ? "" : "s";
        return String.format(Locale.ROOT, "Order {%s}: %d item%s, total %.2f", name, count, ending, count * price);
    }

    public static void main(String[] args) {
        System.out.println(describeOrder("Ada", 3, 1.5));
    }
}
