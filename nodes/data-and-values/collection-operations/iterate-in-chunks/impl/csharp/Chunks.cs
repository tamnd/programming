// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-in-chunks/.

using System.Linq;

public static class Chunks
{
    // Split returns the chunks of items with size items each; the last chunk can be shorter.
    public static int[][] Split(int[] items, int size)
    {
        return items.Chunk(size).ToArray();
    }
}
