// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-two-lists/.

public static class ConcatLists
{
    // The collection expression copies the items now. a.Concat(b) would read
    // the arrays later, when the result is enumerated.
    public static int[] Of(int[] a, int[] b) => [.. a, .. b];
}
