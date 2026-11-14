// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/align-columns-of-text/.

using System.Text;

public static class AlignColumns
{
    // Aligns the fields of the text in columns. A $ separates two fields, and
    // align is "left", "right" or "center". Throws ArgumentException if align
    // is not one of these words.
    public static string Align(string text, string align)
    {
        if (align is not ("left" or "right" or "center"))
        {
            throw new ArgumentException("align must be left, right or center", nameof(align));
        }
        var rows = new List<List<string>>();
        var widths = new List<int>();
        foreach (string line in Split(text, '\n'))
        {
            List<string> fields = Split(line, '$');
            rows.Add(fields);
            for (int j = 0; j < fields.Count; j++)
            {
                int width = Width(fields[j]);
                if (j == widths.Count)
                {
                    widths.Add(width);
                }
                else if (width > widths[j])
                {
                    widths[j] = width;
                }
            }
        }
        var output = new StringBuilder();
        foreach (List<string> fields in rows)
        {
            for (int j = 0; j < fields.Count; j++)
            {
                int pad = widths[j] - Width(fields[j]);
                int left = align switch { "left" => 0, "right" => pad, _ => pad / 2 };
                if (j > 0)
                {
                    output.Append(' ');
                }
                output.Append(' ', left).Append(fields[j]);
                if (j + 1 < fields.Count)
                {
                    output.Append(' ', pad - left);
                }
            }
            output.Append('\n');
        }
        return output.ToString();
    }

    // Returns the number of code points in s. String.Length counts UTF-16
    // code units.
    private static int Width(string s) => s.EnumerateRunes().Count();

    // Splits s at each sep. A sep at the end does not start a new part.
    private static List<string> Split(string s, char sep)
    {
        var parts = new List<string>();
        int start = 0;
        while (start < s.Length)
        {
            int at = s.IndexOf(sep, start);
            if (at < 0)
            {
                at = s.Length;
            }
            parts.Add(s[start..at]);
            start = at + 1;
        }
        return parts;
    }
}
