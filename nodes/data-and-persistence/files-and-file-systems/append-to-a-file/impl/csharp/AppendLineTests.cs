// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-file/.

using System.Text;
using Xunit;

public sealed class AppendLineTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("append-line-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    // name, old content or null, line, content after
    public static TheoryData<string, string?, string, string> Cases => new()
    {
        { "new-file", null, "first", "first\n" },
        { "append-existing", "one\n", "two", "one\ntwo\n" },
        { "new-file-empty-line", null, "", "\n" },
        { "empty-line", "a\n", "", "a\n\n" },
        { "empty-file", "", "x", "x\n" },
        { "no-final-newline", "one", "two", "onetwo\n" },
        { "crlf-file", "one\r\n", "two", "one\r\ntwo\n" },
        { "whitespace", "", "  two spaces\ttab ", "  two spaces\ttab \n" },
        { "non-ascii", "naïve\n", "café 日本語 🙂", "naïve\ncafé 日本語 🙂\n" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Appends(string name, string? old, string line, string want)
    {
        string path = Path.Combine(dir, "log.txt");
        if (old != null) File.WriteAllBytes(path, Encoding.UTF8.GetBytes(old));
        AppendLine.Append(path, line);
        Assert.Equal(want, Encoding.UTF8.GetString(File.ReadAllBytes(path)));
        Assert.False(string.IsNullOrEmpty(name));
    }

    [Fact]
    public void MissingDirectory() =>
        Assert.Throws<DirectoryNotFoundException>(() => AppendLine.Append(Path.Combine(dir, "missing", "log.txt"), "x"));

    [Theory]
    [InlineData("a\nb")]
    [InlineData("a\rb")]
    public void InvalidLine(string line)
    {
        string path = Path.Combine(dir, "log.txt");
        Assert.Throws<ArgumentException>(() => AppendLine.Append(path, line));
        Assert.False(File.Exists(path));
    }

    private static string Text(int w, int i) => $"writer {w} line {i:D3} " + new string('x', 40);

    [Fact]
    public void ManyWriters()
    {
        string path = Path.Combine(dir, "log.txt");
        const int writers = 8, lines = 250;
        var threads = Enumerable.Range(0, writers)
            .Select(w => new Thread(() => { for (int i = 0; i < lines; i++) AppendLine.Append(path, Text(w, i)); }))
            .ToList();
        threads.ForEach(t => t.Start());
        threads.ForEach(t => t.Join());
        var want = Enumerable.Range(0, writers).SelectMany(w => Enumerable.Range(0, lines).Select(i => Text(w, i))).Order();
        Assert.Equal(want, File.ReadAllLines(path).Order());
    }
}
