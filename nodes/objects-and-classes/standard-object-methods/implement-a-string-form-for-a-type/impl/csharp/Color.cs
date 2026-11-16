// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-a-string-form-for-a-type/.

using System;
using System.Collections.Generic;

/// <summary>A color whose string form is the hexadecimal notation, such as #FF8000.</summary>
public sealed record Color
{
    public int R { get; }
    public int G { get; }
    public int B { get; }

    public Color(int r, int g, int b)
    {
        foreach (var v in new[] { r, g, b })
        {
            if (v < 0 || v > 255)
            {
                throw new ArgumentOutOfRangeException(nameof(r), v, "not a channel value");
            }
        }
        (R, G, B) = (r, g, b);
    }

    // The display form. Interpolation, string.Join and Console.WriteLine call it.
    // A record generates ToString from its properties; this override replaces it.
    public override string ToString() => $"#{R:X2}{G:X2}{B:X2}";

    public static List<string> FormatColors(IReadOnlyList<int> reds, IReadOnlyList<int> greens, IReadOnlyList<int> blues)
    {
        if (reds.Count != greens.Count || reds.Count != blues.Count)
        {
            throw new ArgumentException("the lists must have the same length");
        }
        var output = new List<string>(reds.Count);
        for (var i = 0; i < reds.Count; i++)
        {
            output.Add($"{new Color(reds[i], greens[i], blues[i])}");
        }
        return output;
    }
}
