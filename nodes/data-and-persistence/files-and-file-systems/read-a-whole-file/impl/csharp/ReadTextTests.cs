// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-whole-file/.

using System.Text;
using Xunit;

public sealed class ReadTextTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("read-text-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    private string Write(byte[] data)
    {
        string path = Path.Combine(dir, "input.txt");
        File.WriteAllBytes(path, data);
        return path;
    }

    public static TheoryData<string, string> Cases => new()
    {
        { "lf", "alpha\nbeta\n" },
        { "paragraphs", "first line\n\nsecond paragraph\n" },
        { "non-ascii", "naïve café\n日本語\n🙂\n" },
        { "empty", "" },
        { "no-final-newline", "one\ntwo" },
        { "trailing-newlines", "end\n\n\n" },
        { "crlf", "one\r\ntwo\r\n" },
        { "mixed-endings", "lf\ncrlf\r\ncr\rend" },
        { "bom", "\uFEFFhead\n" },
        { "whitespace", "  two spaces\ttab \n" },
        { "large", string.Concat(Enumerable.Repeat("0123456789abcdef", 65536)) },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void ReadsText(string name, string text)
    {
        Assert.Equal(text, ReadText.Read(Write(Encoding.UTF8.GetBytes(text))));
        Assert.False(string.IsNullOrEmpty(name));
    }

    [Fact]
    public void MissingFile() =>
        Assert.Throws<FileNotFoundException>(() => ReadText.Read(Path.Combine(dir, "missing.txt")));

    [Fact]
    public void PathIsADirectory() =>
        Assert.ThrowsAny<Exception>(() => ReadText.Read(dir));

    public static TheoryData<string, byte[]> InvalidUtf8 => new()
    {
        { "invalid-byte", [0x6f, 0x6b, 0x0a, 0xff, 0xfe, 0x0a] },
        { "truncated-at-end", [0x6f, 0x6b, 0x0a, 0x63, 0x61, 0x66, 0xc3] },
        { "encoded-surrogate", [0x61, 0xed, 0xa0, 0x80, 0x62, 0x0a] },
        { "overlong", [0xc0, 0xaf, 0x0a] },
    };

    [Theory]
    [MemberData(nameof(InvalidUtf8))]
    public void RejectsInvalidUtf8(string name, byte[] data)
    {
        Assert.Throws<DecoderFallbackException>(() => ReadText.Read(Write(data)));
        Assert.False(string.IsNullOrEmpty(name));
    }
}
