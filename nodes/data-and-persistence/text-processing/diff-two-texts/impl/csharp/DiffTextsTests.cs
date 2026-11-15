// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/diff-two-texts/.

using System.Text;
using Xunit;

public sealed class DiffTextsTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("diff-two-texts-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    // name, the old text, the new text, the edit script
    public static TheoryData<string, string, string, string> Cases => new()
    {
        { "simple", "a\nb\nc\n", "a\nc\nd\n", " a\n-b\n c\n+d\n" },
        { "change-a-line", "one\ntwo\nthree\n", "one\n2\nthree\n", " one\n-two\n+2\n three\n" },
        { "identical", "a\nb\n", "a\nb\n", " a\n b\n" },
        { "lines-of-a-poem", "The cat sat\non the mat.\nIt was flat.\n", "The cat sat\non the hat.\nIt was flat.\nThe end.\n", " The cat sat\n-on the mat.\n+on the hat.\n It was flat.\n+The end.\n" },
        { "both-empty", "", "", "" },
        { "old-empty", "", "a\nb\n", "+a\n+b\n" },
        { "new-empty", "a\nb\n", "", "-a\n-b\n" },
        { "all-different", "a\nb\n", "c\nd\n", "-a\n-b\n+c\n+d\n" },
        { "insert-at-start", "b\nc\n", "a\nb\nc\n", "+a\n b\n c\n" },
        { "delete-at-end", "a\nb\nc\n", "a\nb\n", " a\n b\n-c\n" },
        { "swap", "a\nb\n", "b\na\n", "-a\n b\n+a\n" },
        { "repeated-lines", "a\na\nb\n", "a\nb\nb\n", " a\n-a\n b\n+b\n" },
        { "empty-line", "a\n\nb\n", "a\nb\n", " a\n-\n b\n" },
        { "no-final-newline", "a\nb", "a\nb\n", " a\n b\n" },
        { "crlf", "a\r\nb\n", "a\nb\n", "-a\r\n+a\n b\n" },
        { "trailing-space", "a \nb\n", "a\nb\n", "-a \n+a\n b\n" },
        { "normalization", "caf\u00e9\n", "cafe\u0301\n", "-caf\u00e9\n+cafe\u0301\n" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Lines(string name, string oldText, string newText, string want)
    {
        Assert.Equal(want, DiffTexts.Lines(oldText, newText));
        _ = name;
    }

    [Fact]
    public void ManyLines()
    {
        // 1000 lines, and 10 of them change.
        string oldText = string.Concat(Enumerable.Range(0, 1000).Select(i => $"{i}\n"));
        string newText = string.Concat(Enumerable.Range(0, 1000).Select(i => i % 100 == 50 ? $"x{i}\n" : $"{i}\n"));
        List<string> lines = DiffTexts.SplitLines(DiffTexts.Lines(oldText, newText));
        Assert.Equal(1010, lines.Count);
        Assert.Equal(" 49,-50,+x50", string.Join(",", lines.GetRange(49, 3)));
        Assert.Equal(" 999", lines[1009]);
    }

    [Fact]
    public void Files()
    {
        // UTF8Encoding(false) writes no byte order mark.
        File.WriteAllText(Path.Combine(dir, "old.txt"), "a\r\nb\n", new UTF8Encoding(false));
        File.WriteAllText(Path.Combine(dir, "new.txt"), "a\nb\n", new UTF8Encoding(false));
        Assert.Equal("-a\r\n+a\n b\n", DiffTexts.Files(Path.Combine(dir, "old.txt"), Path.Combine(dir, "new.txt")));
    }

    [Fact]
    public void NotFound()
    {
        File.WriteAllText(Path.Combine(dir, "new.txt"), "a\n", new UTF8Encoding(false));
        Assert.Throws<FileNotFoundException>(() => DiffTexts.Files(Path.Combine(dir, "missing.txt"), Path.Combine(dir, "new.txt")));
    }
}
