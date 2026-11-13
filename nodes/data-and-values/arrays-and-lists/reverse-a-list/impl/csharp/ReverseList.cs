// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-list/.

public static class ReverseList
{
    // Returns a new array of the items in the opposite order.
    public static int[] Of(int[] items)
    {
        int[] copy = [.. items];
        // Array.Reverse changes the array that it gets, so it gets the copy.
        Array.Reverse(copy);
        return copy;
    }
}
