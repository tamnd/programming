// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.

using System;

/// <summary>
/// Adds WordCount to string. An extension method is a static method whose first
/// parameter has the this modifier, so code can call it as text.WordCount().
/// </summary>
public static class StringExtensions
{
    private static readonly char[] Whitespace = [' ', '\t', '\n', '\r'];

    /// <summary>Returns the number of runs of characters that are not whitespace.</summary>
    public static int WordCount(this string text) =>
        text.Split(Whitespace, StringSplitOptions.RemoveEmptyEntries).Length;
}
