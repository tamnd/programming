// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-a-markdown-text/.

using System.Globalization;
using System.Text;
using System.Text.RegularExpressions;

// Converts a text in a subset of CommonMark to HTML.
public static class MarkdownToHtml
{
    private static readonly Regex Heading = new(@"^(#{1,6})(?:[ \t]+(.*?))?[ \t]*$");
    private static readonly Regex Closing = new(@"(?:^|[ \t]+)#+[ \t]*$");
    private static readonly Regex Fence = new(@"^(`{3,})[ \t]*([^`]*?)[ \t]*$");
    private static readonly Regex Item = new(@"^- (.*)$");
    private static readonly Regex Url = new(@"^[A-Za-z0-9\-._~:/?#=&+%]+");
    private const string Punctuation = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

    private static string Escape(string text) =>
        text.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("\"", "&quot;");

    private static bool IsBlank(string line) => line.Trim(' ', '\t').Length == 0;

    private static bool StartsBlock(string line) =>
        IsBlank(line) || Fence.IsMatch(line) || Heading.IsMatch(line) || Item.IsMatch(line);

    public static string Convert(string text)
    {
        string[] lines = text.Length == 0 ? [] : (text.EndsWith('\n') ? text[..^1] : text).Split('\n');
        var output = new StringBuilder();
        int i = 0;
        while (i < lines.Length)
        {
            string line = lines[i];
            Match m;
            if (IsBlank(line))
            {
                i++;
            }
            else if (Fence.IsMatch(line))
            {
                i = CodeBlock(lines, i, output);
            }
            else if ((m = Heading.Match(line)).Success)
            {
                int level = m.Groups[1].Length;
                string content = Closing.Replace(m.Groups[2].Value, "");
                output.Append($"<h{level}>{Inlines(content)}</h{level}>\n");
                i++;
            }
            else if (Item.IsMatch(line))
            {
                i = BulletList(lines, i, output);
            }
            else
            {
                i = Paragraph(lines, i, output);
            }
        }
        return output.ToString();
    }

    private static int CodeBlock(string[] lines, int i, StringBuilder output)
    {
        Match m = Fence.Match(lines[i]);
        string fence = m.Groups[1].Value;
        string[] info = m.Groups[2].Value.Split([' ', '\t'], StringSplitOptions.RemoveEmptyEntries);
        string attr = info.Length > 0 ? $" class=\"language-{Escape(info[0])}\"" : "";
        output.Append($"<pre><code{attr}>");
        for (i++; i < lines.Length; i++)
        {
            string close = lines[i].TrimEnd(' ', '\t');
            if (close.StartsWith(fence, StringComparison.Ordinal) && close.Trim('`').Length == 0)
            {
                i++;
                break;
            }
            output.Append(Escape(lines[i])).Append('\n');
        }
        output.Append("</code></pre>\n");
        return i;
    }

    private static int BulletList(string[] lines, int i, StringBuilder output)
    {
        var items = new List<string>();
        bool loose = false;
        while (i < lines.Length)
        {
            Match m = Item.Match(lines[i]);
            if (!m.Success)
            {
                break;
            }
            items.Add(Inlines(m.Groups[1].Value.Trim(' ', '\t')));
            i++;
            int j = i;
            while (j < lines.Length && IsBlank(lines[j]))
            {
                j++;
            }
            if (j > i && j < lines.Length && Item.IsMatch(lines[j]))
            {
                loose = true;
                i = j;
            }
        }
        output.Append("<ul>\n");
        foreach (string item in items)
        {
            output.Append(loose ? $"<li>\n<p>{item}</p>\n</li>\n" : $"<li>{item}</li>\n");
        }
        output.Append("</ul>\n");
        return i;
    }

    private static int Paragraph(string[] lines, int i, StringBuilder output)
    {
        int start = i;
        for (i++; i < lines.Length && !StartsBlock(lines[i]); i++)
        {
        }
        string text = string.Join("\n", lines[start..i]).TrimEnd(' ', '\t');
        output.Append($"<p>{Inlines(text)}</p>\n");
        return i;
    }

    private static bool IsSpace(Rune r) =>
        r.Value is '\t' or '\n' or '\f' or '\r' || Rune.GetUnicodeCategory(r) == UnicodeCategory.SpaceSeparator;

    private static bool IsPunct(Rune r) => Rune.IsPunctuation(r) || Rune.IsSymbol(r);

    private sealed class Delimiter
    {
        public int Node, Count, Length;
        public bool Open, Close, Active = true;
        public List<string> Opens = [], Closes = [];
    }

    private static string Join(List<Rune> cs, int start, int end)
    {
        var sb = new StringBuilder();
        for (int k = start; k < end; k++)
        {
            sb.Append(cs[k].ToString());
        }
        return sb.ToString();
    }

    // cs is a list of runes, so that a character outside the BMP is one element.
    private static string Inlines(string text)
    {
        var cs = text.EnumerateRunes().ToList();
        var nodes = new List<string>();
        var delims = new List<Delimiter>();
        var buf = new StringBuilder();
        void Flush()
        {
            if (buf.Length > 0)
            {
                nodes.Add(buf.ToString());
                buf.Clear();
            }
        }
        int i = 0;
        while (i < cs.Count)
        {
            int c = cs[i].Value;
            if (c == '\\' && i + 1 < cs.Count && cs[i + 1].Value < 128 && Punctuation.Contains((char)cs[i + 1].Value))
            {
                buf.Append(Escape(cs[i + 1].ToString()));
                i += 2;
            }
            else if (c == '\\' && i + 1 < cs.Count && cs[i + 1].Value == '\n')
            {
                buf.Append("<br />\n");
                i += 2;
            }
            else if (c == '\n')
            {
                string line = buf.ToString();
                string stripped = line.TrimEnd(' ');
                buf.Clear().Append(stripped).Append(line.Length - stripped.Length >= 2 ? "<br />\n" : "\n");
                i++;
            }
            else if (c == '`')
            {
                int j = i;
                while (j < cs.Count && cs[j].Value == '`')
                {
                    j++;
                }
                int close = ClosingRun(cs, j, j - i);
                if (close < 0)
                {
                    buf.Append('`', j - i);
                    i = j;
                    continue;
                }
                string code = Join(cs, j, close).Replace('\n', ' ');
                if (code.Length >= 2 && code[0] == ' ' && code[^1] == ' ' && code.Trim(' ').Length > 0)
                {
                    code = code[1..^1];
                }
                buf.Append($"<code>{Escape(code)}</code>");
                i = close + (j - i);
            }
            else if (c == '[')
            {
                if (!LinkEnd(cs, i, out int close, out int urlEnd))
                {
                    buf.Append('[');
                    i++;
                    continue;
                }
                string label = Inlines(Join(cs, i + 1, close));
                buf.Append($"<a href=\"{Escape(Join(cs, close + 2, urlEnd))}\">{label}</a>");
                i = urlEnd + 1;
            }
            else if (c == '*')
            {
                int j = i;
                while (j < cs.Count && cs[j].Value == '*')
                {
                    j++;
                }
                Rune before = i > 0 ? cs[i - 1] : new Rune('\n');
                Rune after = j < cs.Count ? cs[j] : new Rune('\n');
                bool left = !IsSpace(after) && (!IsPunct(after) || IsSpace(before) || IsPunct(before));
                bool right = !IsSpace(before) && (!IsPunct(before) || IsSpace(after) || IsPunct(after));
                Flush();
                delims.Add(new Delimiter { Node = nodes.Count, Count = j - i, Length = j - i, Open = left, Close = right });
                nodes.Add("");
                i = j;
            }
            else
            {
                buf.Append(Escape(cs[i].ToString()));
                i++;
            }
        }
        Flush();
        Emphasis(delims);
        foreach (Delimiter d in delims)
        {
            var opens = new List<string>(d.Opens);
            opens.Reverse();
            nodes[d.Node] = string.Concat(d.Closes) + new string('*', d.Count) + string.Concat(opens);
        }
        return string.Concat(nodes);
    }

    // ClosingRun returns the index of the next run of exactly n backticks at or after j, or -1.
    private static int ClosingRun(List<Rune> cs, int j, int n)
    {
        while (j < cs.Count)
        {
            if (cs[j].Value != '`')
            {
                j++;
                continue;
            }
            int k = j;
            while (k < cs.Count && cs[k].Value == '`')
            {
                k++;
            }
            if (k - j == n)
            {
                return j;
            }
            j = k;
        }
        return -1;
    }

    private static bool LinkEnd(List<Rune> cs, int i, out int close, out int urlEnd)
    {
        close = -1;
        urlEnd = -1;
        for (int k = i + 1; k < cs.Count; k++)
        {
            if (cs[k].Value == '[')
            {
                return false;
            }
            if (cs[k].Value == ']')
            {
                close = k;
                break;
            }
        }
        if (close < 0 || close + 1 >= cs.Count || cs[close + 1].Value != '(')
        {
            return false;
        }
        Match m = Url.Match(Join(cs, close + 2, cs.Count));
        if (!m.Success)
        {
            return false;
        }
        urlEnd = close + 2 + m.Length;
        return urlEnd < cs.Count && cs[urlEnd].Value == ')';
    }

    private static void Emphasis(List<Delimiter> delims)
    {
        int k = 0;
        while (k < delims.Count)
        {
            Delimiter closer = delims[k];
            if (!closer.Active || !closer.Close)
            {
                k++;
                continue;
            }
            int j = k - 1;
            for (; j >= 0; j--)
            {
                Delimiter o = delims[j];
                if (!o.Active || !o.Open)
                {
                    continue;
                }
                if ((o.Close || closer.Open) && (o.Length + closer.Length) % 3 == 0
                    && !(o.Length % 3 == 0 && closer.Length % 3 == 0))
                {
                    continue;
                }
                break;
            }
            if (j < 0)
            {
                if (!closer.Open)
                {
                    closer.Active = false;
                }
                k++;
                continue;
            }
            Delimiter d = delims[j];
            int use = d.Count >= 2 && closer.Count >= 2 ? 2 : 1;
            string tag = use == 2 ? "strong" : "em";
            d.Count -= use;
            closer.Count -= use;
            d.Opens.Add($"<{tag}>");
            closer.Closes.Add($"</{tag}>");
            for (int m = j + 1; m < k; m++)
            {
                delims[m].Active = false;
            }
            if (d.Count == 0)
            {
                d.Active = false;
            }
            if (closer.Count == 0)
            {
                closer.Active = false;
                k++;
            }
        }
    }
}
