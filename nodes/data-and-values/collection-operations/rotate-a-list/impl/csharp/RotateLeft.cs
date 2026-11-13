// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rotate-a-list/.

public static class RotateLeft
{
    // By returns a new array with the first k items moved to the end.
    public static int[] By(int[] items, int k)
    {
        if (items.Length == 0)
        {
            return [];
        }
        int s = k % items.Length;
        return [.. items[s..], .. items[..s]];
    }
}
