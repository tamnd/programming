// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicate-lines/.

using System.Text;
using Xunit;

public sealed class UniqueLinesTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("unique-lines-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    // 100000 lines with 1000 different lines.
    private static string ManyLines() =>
        string.Concat(Enumerable.Range(0, 100000).Select(i => $"{i % 1000}\n"));

    private static string[] Numbers(int n) =>
        [.. Enumerable.Range(0, n).Select(i => i.ToString())];

    public static TheoryData<string, string, string[]> Cases => new()
    {
        { "simple", "b\na\nb\nc\na\n", ["b", "a", "c"] },
        { "adjacent", "a\na\nb\n", ["a", "b"] },
        { "no-duplicates", "c\nb\na\n", ["c", "b", "a"] },
        { "empty", "", [] },
        { "empty-lines", "\na\n\n\n", ["", "a"] },
        { "all-same", "x\nx\nx\n", ["x"] },
        { "no-final-newline", "a\nb\na", ["a", "b"] },
        { "crlf", "a\r\na\n", ["a\r", "a"] },
        { "case", "A\na\nA\n", ["A", "a"] },
        { "whitespace", "a \na\n a\na \n", ["a ", "a", " a"] },
        // U+00E9 and e with U+0301 look the same, but their code points are not equal.
        { "normalization", "\u00e9\ne\u0301\n\u00e9\n", ["\u00e9", "e\u0301"] },
        { "many-lines", ManyLines(), Numbers(1000) },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Read(string name, string text, string[] want)
    {
        string path = Path.Combine(dir, "input.txt");
        // UTF8Encoding(false) writes no byte order mark.
        File.WriteAllText(path, text, new UTF8Encoding(false));
        Assert.Equal(want, UniqueLines.Read(path));
        _ = name;
    }

    [Fact]
    public void NotFound()
    {
        Assert.Throws<FileNotFoundException>(() => UniqueLines.Read(Path.Combine(dir, "missing.txt")));
    }
}
