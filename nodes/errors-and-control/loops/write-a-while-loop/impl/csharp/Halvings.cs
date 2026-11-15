// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-while-loop/.

public static class Halvings
{
    // Returns n, n / 2, n / 4, ... while the value is greater than 0.
    public static List<int> Of(int n)
    {
        var result = new List<int>();
        while (n > 0)
        {
            result.Add(n);
            n /= 2;
        }
        return result;
    }
}
