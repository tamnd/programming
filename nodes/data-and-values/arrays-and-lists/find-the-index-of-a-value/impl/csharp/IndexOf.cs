// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-index-of-a-value/.

public static class IndexOf
{
    // Returns the first index of value in items, or -1 if it is not there.
    // For an int[], the lower bound is 0, so Array.IndexOf gives -1.
    public static int Of(int[] items, int value) => Array.IndexOf(items, value);
}
