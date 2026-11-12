// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-code-point-of-a-character/.

using System.Text;

public static class CodePoints
{
    // CodePoint returns the code point of the one character in c. (int)c[0]
    // would give only the first UTF-16 code unit of a code point above U+FFFF.
    public static int CodePoint(string c)
    {
        return Rune.GetRuneAt(c, 0).Value;
    }
}
