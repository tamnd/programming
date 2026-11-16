// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-letters-of-a-text/.

import java.util.Arrays;

/** Counts the letters a to z of a text. */
public final class Letters {
    private Letters() {}

    /**
     * Returns the counts of the letters a to z in text, in the order of the
     * alphabet. Upper case letters count as lower case letters. The test on
     * the range keeps letters outside ASCII out of the array.
     */
    public static long[] countLetters(String text) {
        long[] counts = new long[26];
        for (int k = 0; k < text.length(); k++) {
            char c = text.charAt(k);
            if (c >= 'A' && c <= 'Z') {
                c = (char) (c - 'A' + 'a');
            }
            if (c >= 'a' && c <= 'z') {
                counts[c - 'a']++;
            }
        }
        return counts;
    }

    public static void main(String[] args) {
        System.out.println(Arrays.toString(countLetters("Hello, World!")));
    }
}
