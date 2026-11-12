// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.

using System.Collections.Generic;
using System.Text;

public static class StringCharacters
{
    // Characters returns the code points of s. A char is a UTF-16 code unit,
    // so foreach over the string would give each half of a surrogate pair.
    // EnumerateRunes gives whole code points.
    public static string[] Characters(string s)
    {
        var list = new List<string>();
        foreach (Rune r in s.EnumerateRunes())
        {
            list.Add(r.ToString());
        }
        return list.ToArray();
    }
}
