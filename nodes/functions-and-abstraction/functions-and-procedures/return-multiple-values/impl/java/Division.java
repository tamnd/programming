// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/return-multiple-values/.

/** Floor division that returns two results in a record. */
public final class Division {
    private Division() {}

    /** The two results of divMod. */
    public record QuotientAndRemainder(int quotient, int remainder) {}

    /** Returns the quotient and the remainder of the floor division of a by b. */
    public static QuotientAndRemainder divMod(int a, int b) {
        return new QuotientAndRemainder(Math.floorDiv(a, b), Math.floorMod(a, b));
    }
}
