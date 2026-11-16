// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-function/.

/** Functions that keep a number in a range. */
public final class Limits {
    private Limits() {}

    /** Returns value limited to the range from low to high. */
    public static int clamp(int value, int low, int high) {
        if (value < low) {
            return low;
        }
        if (value > high) {
            return high;
        }
        return value;
    }
}
