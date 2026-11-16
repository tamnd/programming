// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-text-to-a-width/.

using System;
using System.Collections.Generic;
using System.Text;

/// <summary>Greedy word wrap.</summary>
public static class Wrap
{
    private static readonly char[] Spaces = { ' ', '\t', '\r', '\n' };

    /// <summary>
    /// Returns the lines of text, each at most width characters long, with
    /// the greedy method. A word longer than width is a line of its own.
    /// </summary>
    public static List<string> Lines(string text, int width)
    {
        var lines = new List<string>();
        var line = new StringBuilder();
        foreach (var word in text.Split(Spaces, StringSplitOptions.RemoveEmptyEntries))
        {
            if (line.Length == 0)
            {
                line.Append(word);
            }
            else if (line.Length + 1 + word.Length <= width)
            {
                line.Append(' ').Append(word);
            }
            else
            {
                lines.Add(line.ToString());
                line.Clear().Append(word);
            }
        }
        if (line.Length > 0)
        {
            lines.Add(line.ToString());
        }
        return lines;
    }
}
