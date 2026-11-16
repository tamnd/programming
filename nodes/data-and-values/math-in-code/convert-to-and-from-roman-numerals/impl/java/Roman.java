// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

/** Conversion between integers and Roman numerals in the standard form. */
public final class Roman {
    private static final int[] VALUES = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    private static final String[] SYMBOLS = {
        "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"
    };

    private Roman() {}

    /** Returns the Roman numeral of n, for 1 <= n <= 3999. */
    public static String toRoman(long n) {
        if (n < 1 || n > 3999) {
            throw new IllegalArgumentException("n is out of range: " + n);
        }
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < VALUES.length; i++) {
            while (n >= VALUES[i]) {
                sb.append(SYMBOLS[i]);
                n -= VALUES[i];
            }
        }
        return sb.toString();
    }

    /** Returns the value of a Roman numeral in the standard form. */
    public static int fromRoman(String s) {
        if (s.isEmpty()) {
            throw new IllegalArgumentException("not a Roman numeral");
        }
        int total = 0;
        for (int i = 0; i < s.length(); i++) {
            int value = value(s.charAt(i));
            if (value == 0) {
                throw new IllegalArgumentException("not a Roman numeral: " + s);
            }
            int next = i + 1 < s.length() ? value(s.charAt(i + 1)) : 0;
            total += value < next ? -value : value;
        }
        if (total < 1 || total > 3999 || !toRoman(total).equals(s)) {
            throw new IllegalArgumentException("not in the standard form: " + s);
        }
        return total;
    }

    private static int value(char c) {
        return switch (c) {
            case 'I' -> 1;
            case 'V' -> 5;
            case 'X' -> 10;
            case 'L' -> 50;
            case 'C' -> 100;
            case 'D' -> 500;
            case 'M' -> 1000;
            default -> 0;
        };
    }
}
