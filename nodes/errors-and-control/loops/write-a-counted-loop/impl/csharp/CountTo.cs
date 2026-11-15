// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-counted-loop/.

public static class CountTo
{
    // Returns the array { 1, 2, ..., n }.
    public static int[] Of(int n)
    {
        var result = new int[n];
        for (int i = 1; i <= n; i++)
        {
            result[i - 1] = i;
        }
        return result;
    }
}
