// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-an-array/.

using System;

public static class Filled
{
    // new int[n] holds n zeros. Array.Fill then sets each item.
    public static int[] Create(int n, int value)
    {
        var a = new int[n];
        Array.Fill(a, value);
        return a;
    }
}
