// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

using System;

/// <summary>Checks whether a string is empty or has only ASCII white space.</summary>
public static class Empty
{
    public static bool IsEmpty(string s) => s.Length == 0;

    // string.IsNullOrWhiteSpace uses char.IsWhiteSpace, which also accepts
    // Unicode spaces, so the code trims the six ASCII characters instead.
    public static bool IsBlank(string s) => s.AsSpan().Trim(" \t\n\v\f\r").IsEmpty;
}
