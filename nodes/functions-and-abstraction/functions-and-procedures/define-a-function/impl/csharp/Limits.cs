// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-function/.

/// <summary>Functions that keep a number in a range.</summary>
public static class Limits
{
    /// <summary>Returns value limited to the range from low to high.</summary>
    public static int Clamp(int value, int low, int high)
    {
        if (value < low)
        {
            return low;
        }
        if (value > high)
        {
            return high;
        }
        return value;
    }
}
