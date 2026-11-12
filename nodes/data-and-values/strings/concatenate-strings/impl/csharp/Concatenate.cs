// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-strings/.

using System.Globalization;

/// <summary>Concatenates two strings, and a string and a number.</summary>
public static class Concatenate
{
    /// <summary>Returns the characters of a followed by the characters of b.</summary>
    public static string Concat(string a, string b) => a + b;

    /// <summary>Returns s followed by the decimal form of n.</summary>
    // s + n would format n with the current culture. The invariant culture
    // gives the same digits and minus sign on every machine.
    public static string ConcatNumber(string s, int n) => s + n.ToString(CultureInfo.InvariantCulture);
}
