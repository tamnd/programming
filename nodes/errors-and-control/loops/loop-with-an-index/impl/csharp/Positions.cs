// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-an-index/.

public static class Positions
{
    // Returns the index of each item that equals target.
    public static List<int> Of(IReadOnlyList<string> items, string target)
    {
        var result = new List<int>();
        for (int i = 0; i < items.Count; i++)
        {
            // string.Equals compares the characters, with the ordinal rules.
            if (string.Equals(items[i], target, StringComparison.Ordinal))
            {
                result.Add(i);
            }
        }
        return result;
    }
}
