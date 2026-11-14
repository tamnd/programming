// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-lines-from-a-file/.

using System.Text;
using Xunit;

public sealed class RemoveLinesTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("remove-lines-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    // 100000 lines with the numbers 0 to 99999.
    private static string Numbers() =>
        string.Concat(Enumerable.Range(0, 100000).Select(i => $"{i}\n"));

    public static TheoryData<string, string, int, int, int, string> Cases => new()
    {
        { "first-two", "a\nb\nc\nd\n", 1, 2, 2, "c\nd\n" },
        { "middle", "a\nb\nc\nd\n", 2, 2, 2, "a\nd\n" },
        { "last", "a\nb\nc\n", 3, 1, 1, "a\nb\n" },
        { "past-the-end", "a\nb\n", 3, 1, 0, "a\nb\n" },
        { "partly-past-the-end", "a\nb\nc\n", 2, 5, 2, "a\n" },
        { "count-zero", "a\nb\n", 1, 0, 0, "a\nb\n" },
        { "all-lines", "a\nb\n", 1, 2, 2, "" },
        { "empty", "", 1, 1, 0, "" },
        { "empty-line", "a\n\nb\n", 2, 1, 1, "a\nb\n" },
        { "no-final-newline", "a\nb", 1, 1, 1, "b" },
        { "remove-incomplete-line", "a\nb", 2, 1, 1, "a\n" },
        { "crlf", "a\r\nb\r\nc\r\n", 2, 1, 1, "a\r\nc\r\n" },
        { "lone-cr", "a\rb\nc\n", 1, 1, 1, "c\n" },
        { "non-ascii", "caf\u00e9\nna\u00efve\nzo\u00eb\n", 2, 1, 1, "caf\u00e9\nzo\u00eb\n" },
        { "large-count", "a\nb\n", 2, int.MaxValue, 1, "a\n" },
        // Lines 2 to 99999 go.
        { "many-lines", Numbers(), 2, 99998, 99998, "0\n99999\n" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Remove(string name, string text, int start, int count, int want, string after)
    {
        string path = Path.Combine(dir, "input.txt");
        // UTF8Encoding(false) writes no byte order mark.
        File.WriteAllText(path, text, new UTF8Encoding(false));
        Assert.Equal(want, RemoveLines.Remove(path, start, count));
        Assert.Equal(after, File.ReadAllText(path, Encoding.UTF8));
        // No temporary file stays in the directory.
        Assert.Single(Directory.GetFiles(dir));
        _ = name;
    }

    [Fact]
    public void KeepsThePermissions()
    {
        if (OperatingSystem.IsWindows())
        {
            return;
        }
        string path = Path.Combine(dir, "input.txt");
        File.WriteAllText(path, "a\nb\n");
        var mode = UnixFileMode.UserRead | UnixFileMode.UserWrite | UnixFileMode.GroupRead;
        File.SetUnixFileMode(path, mode);
        Assert.Equal(1, RemoveLines.Remove(path, 1, 1));
        Assert.Equal(mode, File.GetUnixFileMode(path));
    }

    [Fact]
    public void NotFound()
    {
        Assert.Throws<FileNotFoundException>(() => RemoveLines.Remove(Path.Combine(dir, "missing.txt"), 1, 1));
    }

    [Theory]
    [InlineData(0, 1)]
    [InlineData(1, -1)]
    public void BadRange(int start, int count)
    {
        string path = Path.Combine(dir, "input.txt");
        File.WriteAllText(path, "a\nb\n");
        Assert.Throws<ArgumentOutOfRangeException>(() => RemoveLines.Remove(path, start, count));
        Assert.Equal("a\nb\n", File.ReadAllText(path));
    }
}
