// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-word-count/.

using System.Text;
using Xunit;

public sealed class WordCountTests : IDisposable
{
    private const int Chunk = 64 * 1024;

    private readonly string dir = Directory.CreateTempSubdirectory("word-count-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    private string Write(byte[] data)
    {
        string path = Path.Combine(dir, "input.txt");
        File.WriteAllBytes(path, data);
        return path;
    }

    public static TheoryData<string, string, long, long, long> Cases => new()
    {
        { "simple", "one two three\n", 1, 3, 14 },
        { "multiple-spaces", "  a   b  \n", 1, 2, 10 },
        { "tabs-and-lines", "a\tb\nc\n", 2, 3, 6 },
        { "empty", "", 0, 0, 0 },
        { "only-white-space", " \n\t\n", 2, 0, 4 },
        { "no-final-newline", "one two", 0, 2, 7 },
        { "crlf", "a b\r\nc\r\n", 2, 3, 8 },
        { "non-ascii", "na\u00efve caf\u00e9\n", 1, 2, 13 },
        { "unicode-spaces", "a\u00a0b\u3000c\u2028d\n", 1, 4, 13 },
        { "not-white-space", "a\u200bb\u00adc\ufeffd\n", 1, 1, 13 },
        { "control-separators", "a\u001cb\u001fc\n", 1, 1, 6 },
        { "nel", "a\u0085b\n", 1, 2, 5 },
        // 100000 lines: more than one buffer of 64 KiB.
        { "many-lines", string.Concat(Enumerable.Repeat("a b\n", 100000)), 100000, 200000, 400000 },
        // A long line, and U+3000 at the end of the first 64 KiB.
        { "chunk-boundary", new string('x', Chunk - 1) + "\u3000y\n", 1, 2, Chunk + 4 },
    };

    public static TheoryData<string, byte[]> Invalid => new()
    {
        { "invalid-bytes", [0x6f, 0x6b, 0xff, 0xfe, 0x0a] },
        { "truncated", [0x6f, 0x6b, 0x20, 0xe2, 0x82] },
        { "surrogate", [0x61, 0xed, 0xa0, 0x80, 0x62, 0x0a] },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Counts(string name, string text, long lines, long words, long bytes)
    {
        Assert.False(string.IsNullOrEmpty(name));
        // new UTF8Encoding(false) writes no byte order mark.
        Assert.Equal(new WordCount(lines, words, bytes), WordCount.Read(Write(new UTF8Encoding(false).GetBytes(text))));
    }

    [Theory]
    [MemberData(nameof(Invalid))]
    public void InvalidUtf8(string name, byte[] data)
    {
        Assert.False(string.IsNullOrEmpty(name));
        Assert.Throws<DecoderFallbackException>(() => WordCount.Read(Write(data)));
    }

    [Fact]
    public void MissingFile() =>
        Assert.Throws<FileNotFoundException>(() => WordCount.Read(Path.Combine(dir, "missing.txt")));
}
