// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-variable/.

/// <summary>Declare a variable, initialize it and reassign it.</summary>
public static class Totals
{
    /// <summary>
    /// Returns the sum of values. The variable result has the type long,
    /// which is named. The loop variable uses var, so the compiler infers
    /// its type from the array.
    /// </summary>
    public static long Total(long[] values)
    {
        long result = 0;
        foreach (var value in values)
        {
            result = result + value;
        }
        return result;
    }
}
