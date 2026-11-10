// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-file-line-by-line/.

using System.Text;
using Xunit;

public sealed class LineStatsTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("line-stats-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    private string Write(byte[] data)
    {
        string path = Path.Combine(dir, "input.txt");
        File.WriteAllBytes(path, data);
        return path;
    }

    public static TheoryData<string, string, long, long> Cases => new()
    {
        { "lf", "alpha\nbeta\ngamma\n", 3, 5 },
        { "blank-lines", "first\n\n\nlast line\n", 4, 9 },
        { "empty", "", 0, 0 },
        { "only-newline", "\n", 1, 0 },
        { "no-final-newline", "one\ntwo\nthree", 3, 5 },
        { "crlf", "one\r\ntwo\r\nthree\r\n", 3, 5 },
        { "crlf-no-final-newline", "one\r\ntwo\r\nthree", 3, 5 },
        { "mixed-endings", "one\r\ntwo\nthree\r\n", 3, 5 },
        { "whitespace", "  two spaces\ttab \n", 1, 17 },
        { "backslashes", "C:\\temp\\new\n", 1, 11 },
        { "long-line", new string('x', 1 << 20) + "\nend\n", 2, 1 << 20 },
        { "non-ascii", "naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n", 3, 10 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void CountsLines(string name, string text, long lines, long longest)
    {
        Assert.False(string.IsNullOrEmpty(name));
        Assert.Equal(new LineStats(lines, longest), LineStats.Read(Write(Encoding.UTF8.GetBytes(text))));
    }

    [Fact]
    public void MissingFile() =>
        Assert.Throws<FileNotFoundException>(() => LineStats.Read(Path.Combine(dir, "missing.txt")));

    [Theory]
    [InlineData("invalid-byte", new byte[] { 0x6f, 0x6b, 0x0a, 0xff, 0xfe, 0x0a })]
    [InlineData("truncated-at-end", new byte[] { 0x6f, 0x6b, 0x0a, 0x63, 0x61, 0x66, 0xc3 })]
    [InlineData("encoded-surrogate", new byte[] { 0x61, 0xed, 0xa0, 0x80, 0x62, 0x0a })]
    [InlineData("overlong", new byte[] { 0xc0, 0xaf, 0x0a })]
    public void InvalidUtf8(string name, byte[] data)
    {
        Assert.False(string.IsNullOrEmpty(name));
        Assert.Throws<DecoderFallbackException>(() => LineStats.Read(Write(data)));
    }
}
