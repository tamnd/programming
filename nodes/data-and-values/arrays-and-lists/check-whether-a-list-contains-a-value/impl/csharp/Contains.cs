// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

public static class Contains
{
    // Returns true if some item of items is equal to value. Array.IndexOf
    // compares each item with the default equality comparer of int.
    public static bool Of(int[] items, int value) => Array.IndexOf(items, value) >= 0;
}
