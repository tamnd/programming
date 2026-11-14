// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

using System.Text;
using Xunit;

public sealed class SortedLinesTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("sort-lines-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    private static string[] AThenB(int n) =>
        [.. Enumerable.Repeat("a", n), .. Enumerable.Repeat("b", n)];

    public static TheoryData<string, string, string[]> Cases => new()
    {
        { "simple", "pear\napple\nfig\n", ["apple", "fig", "pear"] },
        { "case", "b\nB\na\nA\n", ["A", "B", "a", "b"] },
        { "numbers", "10\n9\n100\n", ["10", "100", "9"] },
        { "empty", "", [] },
        { "one-empty-line", "\n", [""] },
        { "empty-lines", "b\n\na\n\n", ["", "", "a", "b"] },
        { "duplicates", "b\na\nb\n", ["a", "b", "b"] },
        { "no-final-newline", "b\na", ["a", "b"] },
        { "crlf", "b\r\na\r\n", ["a\r", "b\r"] },
        { "prefix", "ab\na\n", ["a", "ab"] },
        { "whitespace", "b\n a\n\tc\n", ["\tc", " a", "b"] },
        { "non-ascii", "\u00e9\nz\ne\n", ["e", "z", "\u00e9"] },
        // U+FF61 is below U+1F600, but its UTF-16 char is above 0xD83D.
        { "supplementary", "\U0001F600\n\uff61\n", ["\uff61", "\U0001F600"] },
        { "other-line-breaks", "b\u2028a\nb\u001ca\nb\fa\n", ["b\fa", "b\u001ca", "b\u2028a"] },
        { "many-lines", string.Concat(Enumerable.Repeat("b\na\n", 50000)), AThenB(50000) },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Read(string name, string text, string[] want)
    {
        string path = Path.Combine(dir, "input.txt");
        // UTF8Encoding(false) writes no byte order mark.
        File.WriteAllText(path, text, new UTF8Encoding(false));
        Assert.Equal(want, SortedLines.Read(path));
        _ = name;
    }

    [Fact]
    public void NotFound()
    {
        Assert.Throws<FileNotFoundException>(() => SortedLines.Read(Path.Combine(dir, "missing.txt")));
    }
}
