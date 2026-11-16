// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-caesar-or-rot13-cipher/.

/** The Caesar cipher and ROT13. */
public final class Caesar {
    private Caesar() {}

    /**
     * Moves each letter A to Z and a to z shift places along the alphabet,
     * from z back to a. Other characters do not change. A negative shift
     * decodes, and caesar(text, 13) is ROT13. Math.floorMod gives a value
     * from 0 to 25 also for a negative shift.
     */
    public static String caesar(String text, long shift) {
        int k = (int) Math.floorMod(shift, 26L);
        char[] chars = text.toCharArray();
        for (int i = 0; i < chars.length; i++) {
            char c = chars[i];
            if (c >= 'A' && c <= 'Z') {
                chars[i] = (char) ('A' + (c - 'A' + k) % 26);
            } else if (c >= 'a' && c <= 'z') {
                chars[i] = (char) ('a' + (c - 'a' + k) % 26);
            }
        }
        return new String(chars);
    }

    public static void main(String[] args) {
        System.out.println(caesar("Hello, World!", 13));
    }
}
