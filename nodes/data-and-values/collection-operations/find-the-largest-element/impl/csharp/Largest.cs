// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-largest-element/.

using System.Linq;

public static class Largest
{
    // Max throws InvalidOperationException for an empty array.
    public static int Find(int[] items) => items.Max();
}
