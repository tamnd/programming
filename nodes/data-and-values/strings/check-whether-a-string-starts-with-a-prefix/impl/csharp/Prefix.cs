// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-starts-with-a-prefix/.

using System;

/// <summary>Checks whether a string starts or ends with another string.</summary>
public static class Prefix
{
    // StartsWith(string) without a StringComparison uses the rules of the
    // current culture, so the code gives Ordinal to compare the code units.
    public static bool StartsWith(string s, string prefix) =>
        s.StartsWith(prefix, StringComparison.Ordinal);

    public static bool EndsWith(string s, string suffix) =>
        s.EndsWith(suffix, StringComparison.Ordinal);
}
