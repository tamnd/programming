// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-word-frequencies/.

using System.Text;
using Xunit;

public sealed class WordFrequenciesTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("word-frequencies-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    private string Write(byte[] data)
    {
        string path = Path.Combine(dir, "input.txt");
        File.WriteAllBytes(path, data);
        return path;
    }

    private static byte[] Utf8(string s) => Encoding.UTF8.GetBytes(s);

    // Each expected result is a list of "word count" strings.
    public static TheoryData<string, byte[], int, string[]> Cases => new()
    {
        { "simple", Utf8("the cat and the hat\n"), 2, ["the 2", "and 1"] },
        { "case", Utf8("The THE the tHe\n"), 5, ["the 4"] },
        { "punctuation", Utf8("it's well-dressed, isn't it?\n"), 10, ["it 2", "dressed 1", "isn 1", "s 1", "t 1", "well 1"] },
        { "empty", Utf8(""), 3, [] },
        { "no-letters", Utf8("123 ... 42\n-- !!\n"), 3, [] },
        { "ties", Utf8("b a c b a c\n"), 2, ["a 2", "b 2"] },
        { "digits", Utf8("abc123def 42 x9\n"), 10, ["abc 1", "def 1", "x 1"] },
        { "n-zero", Utf8("the cat and the hat\n"), 0, [] },
        { "n-above-count", Utf8("x y x\n"), 10, ["x 2", "y 1"] },
        { "crlf", Utf8("one two\r\ntwo\r\n"), 5, ["two 2", "one 1"] },
        { "non-ascii", Utf8("caf\u00e9 na\u00efve cafe\n"), 10, ["caf 1", "cafe 1", "na 1", "ve 1"] },
        { "leading-separators", Utf8("  ...hello, world. Hello\n"), 5, ["hello 2", "world 1"] },
        { "no-final-newline", Utf8("dog cat dog"), 5, ["dog 2", "cat 1"] },
        { "not-utf8", [0x6f, 0x6b, 0x20, 0xff, 0xfe, 0x20, 0x6f, 0x6b, 0x0a], 5, ["ok 2"] },
        { "many-lines", Utf8(string.Concat(Enumerable.Repeat("a b a\n", 100000))), 3, ["a 200000", "b 100000"] },
        // One line of 100000 words: longer than the buffer of 64 KiB.
        { "long-line", Utf8(string.Concat(Enumerable.Repeat("word ", 100000))), 3, ["word 100000"] },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Read(string name, byte[] data, int n, string[] want)
    {
        var got = WordFrequencies.Read(Write(data), n).Select(p => $"{p.Word} {p.Count}");
        Assert.Equal(want, got);
        _ = name;
    }

    [Fact]
    public void NotFound() =>
        Assert.Throws<FileNotFoundException>(() => WordFrequencies.Read(Path.Combine(dir, "missing.txt"), 3));
}
