// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-a-step/.

public static class CountBy
{
    // Returns start, start + step, ... while the value has not passed stop.
    public static List<int> Of(int start, int stop, int step)
    {
        var result = new List<int>();
        // A long counter: with an int, i + step can wrap past int.MaxValue.
        for (long i = start; step > 0 ? i <= stop : i >= stop; i += step)
        {
            result.Add((int)i);
        }
        return result;
    }
}
