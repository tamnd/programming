// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-fixed-decimals/.

import java.math.BigDecimal;
import java.math.RoundingMode;

/** A number with a fixed count of decimals. */
public final class Fixed {
    private Fixed() {}

    /**
     * Returns x with exactly digits digits after the decimal point. new
     * BigDecimal(x) is the exact binary value of x, so the result is the same
     * as in C or Python. String.format rounds the short decimal text of x
     * instead, and gives 2.68 for 2.675.
     */
    public static String fixed(double x, int digits) {
        return new BigDecimal(x).setScale(digits, RoundingMode.HALF_EVEN).toPlainString();
    }

    public static void main(String[] args) {
        System.out.println(fixed(3.14159, 2));
        System.out.println(fixed(2.675, 2));
    }
}
