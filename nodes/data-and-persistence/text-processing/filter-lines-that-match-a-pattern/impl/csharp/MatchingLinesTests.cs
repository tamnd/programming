// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

using System.Text;
using Xunit;

public sealed class MatchingLinesTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("matching-lines-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    // 100000 lines with the numbers 0 to 99999.
    private static string Numbers() =>
        string.Concat(Enumerable.Range(0, 100000).Select(i => $"{i}\n"));

    public static TheoryData<string, string, string, string[]> Cases => new()
    {
        { "literal", "apple\nbanana\ncherry\n", "an", ["banana"] },
        { "several", "cat\ndog\ncatalog\n", "cat", ["cat", "catalog"] },
        { "start", "ab\nba\n", "^a", ["ab"] },
        { "end", "ab\nba\n", "a$", ["ba"] },
        { "dot", "abc\nac\naxc\n", "a.c", ["abc", "axc"] },
        { "star", "ac\nabc\nabbc\nadc\n", "ab*c", ["ac", "abc", "abbc"] },
        { "whole-line", "a\nab\nba\n", "^a$", ["a"] },
        { "empty", "", "a", [] },
        { "no-match", "abc\n", "x", [] },
        { "empty-lines", "a\n\nb\n\n", "^$", ["", ""] },
        { "match-all", "a\n\nb\n", ".*", ["a", "", "b"] },
        { "duplicates", "a\nb\na\n", "a", ["a", "a"] },
        { "no-final-newline", "a\nab", "b", ["ab"] },
        { "crlf-end", "ab\r\nab\n", "b$", ["ab"] },
        { "crlf-dot", "a\r\n", "^a.$", ["a\r"] },
        { "case", "Apple\napple\n", "apple", ["apple"] },
        { "non-ascii", "caf\u00e9\ncafe\n", "\u00e9", ["caf\u00e9"] },
        // Five of the numbers hold only nines.
        { "many-lines", Numbers(), "^9*$", ["9", "99", "999", "9999", "99999"] },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Read(string name, string text, string pattern, string[] want)
    {
        string path = Path.Combine(dir, "input.txt");
        // UTF8Encoding(false) writes no byte order mark.
        File.WriteAllText(path, text, new UTF8Encoding(false));
        Assert.Equal(want, MatchingLines.Read(path, pattern));
        _ = name;
    }

    [Fact]
    public void NotFound()
    {
        Assert.Throws<FileNotFoundException>(() => MatchingLines.Read(Path.Combine(dir, "missing.txt"), "a"));
    }
}
