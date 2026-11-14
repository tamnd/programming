// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

using System.Text;
using Xunit;

public sealed class SpecificLineTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("specific-line-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    // 100000 lines with the numbers 0 to 99999. Line n holds n - 1.
    private static string Numbers() =>
        string.Concat(Enumerable.Range(0, 100000).Select(i => $"{i}\n"));

    public static TheoryData<string, string, int, string?> Cases => new()
    {
        { "first", "a\nb\nc\n", 1, "a" },
        { "middle", "a\nb\nc\n", 2, "b" },
        { "last", "a\nb\nc\n", 3, "c" },
        { "past-the-end", "a\nb\n", 3, null },
        { "zero", "a\n", 0, null },
        { "negative", "a\n", -1, null },
        { "empty", "", 1, null },
        { "empty-line", "a\n\nb\n", 2, "" },
        { "only-newline", "\n", 1, "" },
        { "no-final-newline", "a\nb", 2, "b" },
        { "crlf", "a\r\nb\r\n", 1, "a\r" },
        { "lone-cr", "a\rb\nc\n", 2, "c" },
        { "non-ascii", "caf\u00e9\nzo\u00eb\n", 2, "zo\u00eb" },
        { "many-lines", Numbers(), 100000, "99999" },
        { "many-lines-past-the-end", Numbers(), 100001, null },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Read(string name, string text, int number, string? want)
    {
        string path = Path.Combine(dir, "input.txt");
        // UTF8Encoding(false) writes no byte order mark.
        File.WriteAllText(path, text, new UTF8Encoding(false));
        Assert.Equal(want, SpecificLine.Read(path, number));
        _ = name;
    }

    [Fact]
    public void NotFound()
    {
        Assert.Throws<FileNotFoundException>(() => SpecificLine.Read(Path.Combine(dir, "missing.txt"), 1));
    }
}
