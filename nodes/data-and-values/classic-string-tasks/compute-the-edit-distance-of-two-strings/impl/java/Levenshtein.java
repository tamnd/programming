// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-edit-distance-of-two-strings/.

/** The Levenshtein distance of two strings. */
public final class Levenshtein {
    private Levenshtein() {}

    /**
     * Returns the minimum number of one-character insertions, deletions and
     * substitutions that change a into b. Two rows of the table are kept.
     */
    public static int levenshtein(String a, String b) {
        int[] prev = new int[b.length() + 1];
        int[] cur = new int[b.length() + 1];
        for (int j = 0; j <= b.length(); j++) {
            prev[j] = j;
        }
        for (int i = 1; i <= a.length(); i++) {
            cur[0] = i;
            for (int j = 1; j <= b.length(); j++) {
                int cost = a.charAt(i - 1) == b.charAt(j - 1) ? 0 : 1;
                cur[j] = Math.min(prev[j - 1] + cost, Math.min(prev[j], cur[j - 1]) + 1);
            }
            int[] t = prev;
            prev = cur;
            cur = t;
        }
        return prev[b.length()];
    }

    public static void main(String[] args) {
        System.out.println(levenshtein("kitten", "sitting"));
    }
}
