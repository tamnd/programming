// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-patch/.

using System.Globalization;
using System.Text;
using System.Text.RegularExpressions;

/// <summary>The patch is malformed, or a hunk does not match the text.</summary>
public sealed class PatchException(PatchException.Kinds kind, int line)
    : Exception($"{kind.ToString().ToLowerInvariant()} at line {line} of the patch")
{
    /// <summary>The kinds of the error.</summary>
    public enum Kinds { Malformed, Mismatch }

    public Kinds Kind { get; } = kind;

    /// <summary>The number of the line of the patch where the error is.</summary>
    public int Line { get; } = line;
}

/// <summary>Applies a unified diff to a text, with no fuzz and no offset.</summary>
public static partial class ApplyPatch
{
    [GeneratedRegex(@"^@@ -(\d+)(?:,(\d+))? \+(\d+)(?:,(\d+))? @@", RegexOptions.CultureInvariant)]
    private static partial Regex Hunk();

    /// <summary>Splits text after each LF. Each line keeps its LF, and a last line with no LF is a line too.</summary>
    public static List<string> SplitLines(string text)
    {
        var lines = new List<string>();
        int start = 0;
        while (start < text.Length)
        {
            int end = text.IndexOf('\n', start);
            end = end < 0 ? text.Length : end + 1;
            lines.Add(text[start..end]);
            start = end;
        }
        return lines;
    }

    private static PatchException Malformed(int line) => new(PatchException.Kinds.Malformed, line);

    private static PatchException Mismatch(int line) => new(PatchException.Kinds.Mismatch, line);

    private static int Count(Group g) => g.Success ? int.Parse(g.Value, CultureInfo.InvariantCulture) : 1;

    /// <summary>
    /// Returns the text that the unified diff patch makes from the text oldText. Each hunk must
    /// match the lines of oldText at the start of its old range. Throws PatchException if the
    /// patch is malformed or a hunk does not match.
    /// </summary>
    public static string Apply(string oldText, string patch)
    {
        List<string> lines = SplitLines(oldText);
        // The lines of the patch, with no LF. A CR stays in its line.
        List<string> p = SplitLines(patch).ConvertAll(l => l.EndsWith('\n') ? l[..^1] : l);
        var output = new StringBuilder();
        int cur = 0; // the index of the next line of oldText to use
        int hunks = 0;
        int i = 0;
        while (i < p.Count)
        {
            if (!p[i].StartsWith("@@", StringComparison.Ordinal))
            {
                i++; // a header line, or another line outside a hunk
                continue;
            }
            Match m = Hunk().Match(p[i]);
            if (!m.Success)
            {
                throw Malformed(i + 1);
            }
            int oldStart, oldCount, newCount;
            try
            {
                oldStart = int.Parse(m.Groups[1].Value, CultureInfo.InvariantCulture);
                oldCount = Count(m.Groups[2]);
                newCount = Count(m.Groups[4]);
            }
            catch (OverflowException)
            {
                throw Malformed(i + 1);
            }
            hunks++;
            // An empty range starts after the line oldStart.
            int pos = oldCount > 0 ? oldStart - 1 : oldStart;
            if (pos < 0)
            {
                throw Malformed(i + 1);
            }
            if (pos < cur || pos > lines.Count)
            {
                throw Mismatch(i + 1);
            }
            for (; cur < pos; cur++)
            {
                output.Append(lines[cur]);
            }
            i++;
            // The mark, the text, the number in the patch and the end of each line: an LF, or ""
            // before a backslash line.
            var body = new List<(char Mark, string Text, int Number)>();
            var ends = new List<string>();
            int oldLeft = oldCount, newLeft = newCount;
            while (oldLeft > 0 || newLeft > 0)
            {
                if (i == p.Count)
                {
                    throw Malformed(i);
                }
                string line = p[i];
                char mark = line.Length == 0 ? ' ' : line[0];
                if (mark == '\\')
                {
                    if (body.Count == 0)
                    {
                        throw Malformed(i + 1);
                    }
                    ends[^1] = ""; // the line before has no LF
                    i++;
                    continue;
                }
                if (mark is not (' ' or '-' or '+'))
                {
                    throw Malformed(i + 1);
                }
                if (mark != '+')
                {
                    oldLeft--;
                }
                if (mark != '-')
                {
                    newLeft--;
                }
                if (oldLeft < 0 || newLeft < 0)
                {
                    throw Malformed(i + 1);
                }
                body.Add((mark, line.Length == 0 ? "" : line[1..], i + 1));
                ends.Add("\n");
                i++;
            }
            if (i < p.Count && p[i].StartsWith('\\') && body.Count > 0)
            {
                ends[^1] = "";
                i++;
            }
            for (int k = 0; k < body.Count; k++)
            {
                var (mark, text, number) = body[k];
                string line = text + ends[k];
                if (mark != '+')
                {
                    if (cur == lines.Count || !string.Equals(lines[cur], line, StringComparison.Ordinal))
                    {
                        throw Mismatch(number);
                    }
                    cur++;
                }
                if (mark != '-')
                {
                    output.Append(line);
                }
            }
        }
        if (hunks == 0)
        {
            throw Malformed(p.Count);
        }
        for (; cur < lines.Count; cur++)
        {
            output.Append(lines[cur]);
        }
        return output.ToString();
    }

    /// <summary>
    /// Returns Apply of the two UTF-8 files. Throws FileNotFoundException if a file does not exist,
    /// and PatchException.
    /// </summary>
    public static string ApplyFile(string path, string patchPath) =>
        Apply(File.ReadAllText(path, Encoding.UTF8), File.ReadAllText(patchPath, Encoding.UTF8));
}
