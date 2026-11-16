// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/destructure-a-value/.

using System;

public static class Manhattan
{
    // Returns the Manhattan distance between the points p = [x, y] and q = [x, y].
    public static int Distance(int[] p, int[] q)
    {
        // A list pattern tests the length and names the elements in one step.
        if (p is [var x1, var y1] && q is [var x2, var y2])
        {
            return Math.Abs(x1 - x2) + Math.Abs(y1 - y2);
        }
        throw new ArgumentException("a point has exactly two coordinates");
    }
}
