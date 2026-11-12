// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-substring/.

public static class Substring
{
    /// <summary>Returns the code points of s from index start up to, not including, end.</summary>
    public static string Get(string s, int start, int end)
    {
        // string.Substring takes char indexes, which are UTF-16 code units,
        // so the code converts the code point indexes first.
        int from = Offset(s, 0, start);
        int to = Offset(s, from, end - start);
        return s[from..to];
    }

    // Offset returns the char index that is count code points after index i.
    private static int Offset(string s, int i, int count)
    {
        for (int k = 0; k < count; k++)
        {
            i += char.IsSurrogatePair(s, i) ? 2 : 1;
        }
        return i;
    }
}
