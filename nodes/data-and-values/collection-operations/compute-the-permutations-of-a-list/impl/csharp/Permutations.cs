// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

using System.Collections.Generic;

public static class Permutations
{
    // Of returns each order of the items, in the lexicographic order of the positions.
    public static int[][] Of(int[] items)
    {
        var output = new List<int[]>();
        Visit(items, new bool[items.Length], new int[items.Length], 0, output);
        return output.ToArray();
    }

    private static void Visit(int[] items, bool[] used, int[] path, int depth, List<int[]> output)
    {
        if (depth == items.Length)
        {
            // path changes later, so add a copy.
            output.Add((int[])path.Clone());
            return;
        }
        for (int i = 0; i < items.Length; i++)
        {
            if (used[i])
            {
                continue;
            }
            used[i] = true;
            path[depth] = items[i];
            Visit(items, used, path, depth + 1, output);
            used[i] = false;
        }
    }
}
