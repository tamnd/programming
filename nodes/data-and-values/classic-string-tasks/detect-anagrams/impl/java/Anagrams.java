// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-anagrams/.

/** Checks whether two texts are anagrams. */
public final class Anagrams {
    private Anagrams() {}

    /** Returns the index 0 to 25 of an ASCII letter, or -1 for every other character. */
    private static int index(char c) {
        if (c >= 'a' && c <= 'z') {
            return c - 'a';
        }
        if (c >= 'A' && c <= 'Z') {
            return c - 'A';
        }
        return -1;
    }

    /**
     * Returns true if a and b hold each letter from a to z the same number of
     * times. Case, spaces, punctuation and digits do not count.
     */
    public static boolean areAnagrams(String a, String b) {
        int[] counts = new int[26];
        for (int k = 0; k < a.length(); k++) {
            int i = index(a.charAt(k));
            if (i >= 0) {
                counts[i]++;
            }
        }
        for (int k = 0; k < b.length(); k++) {
            int i = index(b.charAt(k));
            if (i >= 0) {
                counts[i]--;
            }
        }
        for (int n : counts) {
            if (n != 0) {
                return false;
            }
        }
        return true;
    }

    public static void main(String[] args) {
        System.out.println(areAnagrams("Dormitory", "dirty room"));
    }
}
