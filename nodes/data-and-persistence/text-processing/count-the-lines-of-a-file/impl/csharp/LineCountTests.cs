// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

using System.Text;
using Xunit;

public sealed class LineCountTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("count-lines-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    private string Write(byte[] data)
    {
        string path = Path.Combine(dir, "input.txt");
        File.WriteAllBytes(path, data);
        return path;
    }

    public static TheoryData<string, string, long, long> Cases => new()
    {
        { "lf", "alpha\nbeta\ngamma\n", 3, 3 },
        { "blank-lines", "first\n\n\nlast\n", 4, 4 },
        { "empty", "", 0, 0 },
        { "only-newline", "\n", 1, 1 },
        { "no-final-newline", "one\ntwo\nthree", 2, 3 },
        { "one-line-no-newline", "x", 0, 1 },
        { "crlf", "one\r\ntwo\r\n", 2, 2 },
        { "crlf-no-final-newline", "one\r\ntwo", 1, 2 },
        { "cr-only", "one\rtwo\r", 0, 1 },
        { "other-breaks", "a\vb\fc\u0085d\u2028e\n", 1, 1 },
        { "nul-bytes", "a\0b\n\0\n", 2, 2 },
        // 100000 lines and a last line with no LF: more than one buffer of 64 KiB.
        { "many-chunks", string.Concat(Enumerable.Repeat("x\n", 100000)) + "end", 100000, 100001 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void CountsLines(string name, string text, long newlines, long lines)
    {
        Assert.False(string.IsNullOrEmpty(name));
        Assert.Equal(new LineCount(newlines, lines), LineCount.Read(Write(Encoding.UTF8.GetBytes(text))));
    }

    [Fact]
    public void InvalidUtf8() =>
        Assert.Equal(new LineCount(2, 2), LineCount.Read(Write([0x6f, 0x6b, 0x0a, 0xff, 0xfe, 0x0a])));

    [Fact]
    public void MissingFile() =>
        Assert.Throws<FileNotFoundException>(() => LineCount.Read(Path.Combine(dir, "missing.txt")));
}
