// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/swap-two-values/.

/** Swap two values with a temporary variable. */
public final class Swap {
    private Swap() {}

    /**
     * Returns a copy of xs with the items at i and j swapped. Java has no
     * parallel assignment, so the swap keeps one value in a temporary
     * variable.
     */
    public static long[] swapItems(long[] xs, int i, int j) {
        long[] out = xs.clone();
        long tmp = out[i];
        out[i] = out[j];
        out[j] = tmp;
        return out;
    }

    public static void main(String[] args) {
        System.out.println(java.util.Arrays.toString(swapItems(new long[] {1, 2, 3}, 0, 2)));
    }
}
