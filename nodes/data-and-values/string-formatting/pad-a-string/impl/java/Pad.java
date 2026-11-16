// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pad-a-string/.

/** Padding of a string on the left and on the right. */
public final class Pad {
    private Pad() {}

    /**
     * Returns s padded with fill to width: element 0 is padded on the left
     * and element 1 on the right. If s has width characters or more, both
     * elements are s.
     */
    public static String[] pad(String s, int width, String fill) {
        String extra = fill.repeat(Math.max(0, width - s.length()));
        return new String[] {extra + s, s + extra};
    }

    public static void main(String[] args) {
        String[] r = pad("7", 3, "0");
        System.out.println(r[0] + " " + r[1]);
    }
}
