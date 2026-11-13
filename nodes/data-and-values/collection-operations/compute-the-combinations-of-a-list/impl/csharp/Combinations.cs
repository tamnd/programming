// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

using System.Collections.Generic;

public static class Combinations
{
    // Of returns each choice of k items, in the lexicographic order of the positions.
    public static int[][] Of(int[] items, int k)
    {
        var output = new List<int[]>();
        if (k <= items.Length)
        {
            Visit(items, new int[k], 0, 0, output);
        }
        return output.ToArray();
    }

    private static void Visit(int[] items, int[] path, int depth, int start, List<int[]> output)
    {
        if (depth == path.Length)
        {
            // path changes later, so add a copy.
            output.Add((int[])path.Clone());
            return;
        }
        // Stop when too few items are left to fill path.
        for (int i = start; i <= items.Length - (path.Length - depth); i++)
        {
            path[depth] = items[i];
            Visit(items, path, depth + 1, i + 1, output);
        }
    }
}
