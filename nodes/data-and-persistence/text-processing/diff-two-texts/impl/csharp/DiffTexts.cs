// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/diff-two-texts/.

using System.Text;

/// <summary>Compares two texts line by line.</summary>
public static class DiffTexts
{
    /// <summary>Splits text at each LF. A final LF does not start a new line.</summary>
    public static List<string> SplitLines(string text)
    {
        var lines = new List<string>(text.Split('\n'));
        // "a\n".Split('\n') is ["a", ""], and "".Split('\n') is [""].
        if (lines[^1].Length == 0)
        {
            lines.RemoveAt(lines.Count - 1);
        }
        return lines;
    }

    /// <summary>
    /// Returns the edit script that changes the lines of oldText into the lines of newText. Each
    /// line of the result is a space and a line that stays, a - and a line of oldText that goes,
    /// or a + and a line of newText that comes, with an LF at the end. The script has the smallest
    /// number of - and + lines. When two scripts are equally short, a - comes before a +.
    /// </summary>
    public static string Lines(string oldText, string newText)
    {
        List<string> a = SplitLines(oldText), b = SplitLines(newText);
        int n = a.Count, m = b.Count, w = m + 1;
        // lcs[i * w + j] is the length of a longest common subsequence of the lines from a[i]
        // and from b[j]. Row n and column m stay 0.
        var lcs = new int[checked((n + 1) * w)];
        for (int i = n - 1; i >= 0; i--)
        {
            for (int j = m - 1; j >= 0; j--)
            {
                lcs[i * w + j] = string.Equals(a[i], b[j], StringComparison.Ordinal)
                    ? lcs[(i + 1) * w + j + 1] + 1
                    : Math.Max(lcs[(i + 1) * w + j], lcs[i * w + j + 1]);
            }
        }
        var output = new StringBuilder();
        void Line(char mark, string s) => output.Append(mark).Append(s).Append('\n');
        int x = 0, y = 0;
        while (x < n && y < m)
        {
            if (string.Equals(a[x], b[y], StringComparison.Ordinal))
            {
                Line(' ', a[x++]);
                y++;
            }
            else if (lcs[(x + 1) * w + y] >= lcs[x * w + y + 1])
            {
                Line('-', a[x++]);
            }
            else
            {
                Line('+', b[y++]);
            }
        }
        while (x < n)
        {
            Line('-', a[x++]);
        }
        while (y < m)
        {
            Line('+', b[y++]);
        }
        return output.ToString();
    }

    /// <summary>Returns <see cref="Lines"/> of the two UTF-8 files.</summary>
    /// <exception cref="FileNotFoundException">A file does not exist.</exception>
    public static string Files(string oldPath, string newPath) =>
        Lines(File.ReadAllText(oldPath, Encoding.UTF8), File.ReadAllText(newPath, Encoding.UTF8));
}
