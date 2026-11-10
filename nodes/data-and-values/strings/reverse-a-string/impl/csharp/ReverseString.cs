// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-string/.

using System.Globalization;
using System.Text;

public static class ReverseString
{
    // Returns the code points of s in the opposite order. EnumerateRunes
    // yields Unicode scalar values, so a surrogate pair stays together.
    // Array.Reverse on s.ToCharArray() would break the pair.
    public static string ReverseCodePoints(string s)
    {
        var builder = new StringBuilder(s.Length);
        foreach (Rune rune in s.EnumerateRunes().Reverse())
        {
            builder.Append(rune);
        }
        return builder.ToString();
    }

    // Returns the text elements of s in the opposite order. The API reference
    // calls a text element an extended grapheme cluster. Before .NET 5,
    // StringInfo handled some grapheme clusters incorrectly.
    public static string ReverseGraphemes(string s)
    {
        var parts = new List<string>();
        TextElementEnumerator elements = StringInfo.GetTextElementEnumerator(s);
        while (elements.MoveNext())
        {
            parts.Add(elements.GetTextElement());
        }
        parts.Reverse();
        return string.Concat(parts);
    }
}
