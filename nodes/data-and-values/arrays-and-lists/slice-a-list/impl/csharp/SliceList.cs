// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/slice-a-list/.

public static class SliceList
{
    // Returns a new array of items[start] up to, but not including, items[stop].
    // The range of an array is a copy, and a bad range throws
    // ArgumentOutOfRangeException.
    public static int[] Of(int[] items, int start, int stop) => items[start..stop];
}
