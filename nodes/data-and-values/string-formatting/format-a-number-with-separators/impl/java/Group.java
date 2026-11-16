// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-separators/.

import java.util.Locale;

/** An integer with a separator between each group of 3 digits. */
public final class Group {
    private Group() {}

    /**
     * Returns n in decimal with a comma between each group of 3 digits.
     * Locale.ROOT gives the comma on each computer.
     */
    public static String groupThousands(long n) {
        return String.format(Locale.ROOT, "%,d", n);
    }

    public static void main(String[] args) {
        System.out.println(groupThousands(1234567));
    }
}
