// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-binary-file/.

using Xunit;

public sealed class ReadBytesTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("read-bytes-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    private static readonly Dictionary<string, byte[]> Data = new()
    {
        ["signature"] = Convert.FromHexString("89504e470d0a1a0a0000000d49484452"),
        ["bytes"] = Convert.FromHexString("0001027f80feff"),
        ["empty"] = [],
        ["all-bytes"] = Enumerable.Range(0, 256).Select(i => (byte)i).ToArray(),
        ["zeros"] = new byte[8],
        ["crlf"] = "a\r\nb\r\n"u8.ToArray(),
        ["ctrl-z"] = Convert.FromHexString("611a62"),
        ["not-utf8"] = Convert.FromHexString("c328fffe80"),
        ["large"] = Enumerable.Range(0, 1 << 20).Select(i => (byte)i).ToArray(),
    };

    private string Write(string name)
    {
        string path = Path.Combine(dir, "input.bin");
        File.WriteAllBytes(path, Data[name]);
        return path;
    }

    public static TheoryData<string> Cases => new(Data.Keys);

    [Theory]
    [MemberData(nameof(Cases))]
    public void ReadsBytes(string name) => Assert.Equal(Data[name], ReadBytes.Read(Write(name)));

    public static TheoryData<string, string, int, string> Exact => new()
    {
        { "signature-only", "signature", 8, "89504e470d0a1a0a" },
        { "first-four", "all-bytes", 4, "00010203" },
        { "whole-file", "zeros", 8, "0000000000000000" },
        { "zero-count", "empty", 0, "" },
        { "across-ctrl-z", "ctrl-z", 3, "611a62" },
    };

    [Theory]
    [MemberData(nameof(Exact))]
    public void ReadsExact(string name, string data, int n, string want)
    {
        Assert.Equal(Convert.FromHexString(want), ReadBytes.ReadExact(Write(data), n));
        Assert.False(string.IsNullOrEmpty(name));
    }

    [Fact]
    public void ReadsExactLarge() => Assert.Equal(Data["large"], ReadBytes.ReadExact(Write("large"), 1 << 20));

    [Theory]
    [InlineData("one-past-end", "signature", 17)]
    [InlineData("empty-file", "empty", 1)]
    public void ShortFile(string name, string data, int n)
    {
        string path = Write(data);
        Assert.Throws<EndOfStreamException>(() => ReadBytes.ReadExact(path, n));
        Assert.False(string.IsNullOrEmpty(name));
    }

    [Fact]
    public void MissingFile()
    {
        string path = Path.Combine(dir, "missing.bin");
        Assert.Throws<FileNotFoundException>(() => ReadBytes.Read(path));
        Assert.Throws<FileNotFoundException>(() => ReadBytes.ReadExact(path, 1));
    }

    [Fact]
    public void PathIsADirectory()
    {
        Assert.Throws<UnauthorizedAccessException>(() => ReadBytes.Read(dir));
        Assert.Throws<UnauthorizedAccessException>(() => ReadBytes.ReadExact(dir, 1));
    }
}
