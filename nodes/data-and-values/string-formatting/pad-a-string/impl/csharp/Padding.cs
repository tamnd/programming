// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pad-a-string/.

/// <summary>Padding of a string on the left and on the right.</summary>
public static class Padding
{
    /// <summary>
    /// Returns s padded with the first character of fill to width, once on
    /// the left and once on the right. If s has width characters or more,
    /// both results are s.
    /// </summary>
    public static (string Left, string Right) Pad(string s, int width, string fill) =>
        (s.PadLeft(width, fill[0]), s.PadRight(width, fill[0]));
}
