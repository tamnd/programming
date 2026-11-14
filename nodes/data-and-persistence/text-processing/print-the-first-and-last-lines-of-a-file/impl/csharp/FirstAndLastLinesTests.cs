// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

using System.Text;
using Xunit;

public sealed class FirstAndLastLinesTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("first-and-last-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    // The numbers from and up to to, but not to.
    private static string[] Numbers(int from, int to) =>
        Enumerable.Range(from, to - from).Select(i => $"{i}").ToArray();

    private static string Text(string[] lines) => string.Concat(lines.Select(l => l + "\n"));

    public static TheoryData<string, string, int, string[], string[]> Cases => new()
    {
        { "two", "a\nb\nc\nd\ne\n", 2, ["a", "b"], ["d", "e"] },
        { "one", "first\nmiddle\nlast\n", 1, ["first"], ["last"] },
        { "ten", Text(Numbers(1, 13)), 10, Numbers(1, 11), Numbers(3, 13) },
        { "empty", "", 2, [], [] },
        { "zero", "a\nb\n", 0, [], [] },
        { "short", "a\nb\n", 5, ["a", "b"], ["a", "b"] },
        { "exact", "a\nb\n", 2, ["a", "b"], ["a", "b"] },
        { "overlap", "a\nb\nc\n", 2, ["a", "b"], ["b", "c"] },
        { "one-line", "a\n", 1, ["a"], ["a"] },
        { "empty-lines", "\n\na\n\n", 1, [""], [""] },
        { "only-newline", "\n", 1, [""], [""] },
        { "no-final-newline", "a\nb\nc", 1, ["a"], ["c"] },
        { "crlf", "a\r\nb\r\n", 1, ["a\r"], ["b\r"] },
        { "non-ascii", "caf\u00e9\nna\u00efve\nzo\u00eb\n", 1, ["caf\u00e9"], ["zo\u00eb"] },
        // 100000 lines with the numbers 0 to 99999.
        { "many-lines", Text(Numbers(0, 100000)), 3, Numbers(0, 3), Numbers(99997, 100000) },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Read(string name, string text, int count, string[] first, string[] last)
    {
        string path = Path.Combine(dir, "input.txt");
        // UTF8Encoding(false) writes no byte order mark.
        File.WriteAllText(path, text, new UTF8Encoding(false));
        var got = FirstAndLastLines.Read(path, count);
        Assert.Equal(first, got.First);
        Assert.Equal(last, got.Last);
        _ = name;
    }

    [Fact]
    public void NotFound()
    {
        Assert.Throws<FileNotFoundException>(() => FirstAndLastLines.Read(Path.Combine(dir, "missing.txt"), 1));
    }
}
