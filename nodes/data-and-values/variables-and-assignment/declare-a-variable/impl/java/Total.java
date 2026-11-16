// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-variable/.

/** Declare a variable, initialize it and reassign it. */
public final class Total {
    private Total() {}

    /**
     * Returns the sum of values. The variable result has the type long,
     * which is named. The loop variable uses var, so the compiler infers
     * its type from the array.
     */
    public static long total(long[] values) {
        long result = 0;
        for (var value : values) {
            result = result + value;
        }
        return result;
    }

    public static void main(String[] args) {
        System.out.println(total(new long[] {1, 2, 3}));
    }
}
