// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-list/.

public static class AppendToList
{
    // Returns a new array with value at position index. index items.Length appends.
    // The range items[..index] throws ArgumentOutOfRangeException for a bad index.
    // For a List<T>, list.Insert(index, value) does the same in place.
    public static int[] InsertAt(int[] items, int index, int value) =>
        [.. items[..index], value, .. items[index..]];
}
