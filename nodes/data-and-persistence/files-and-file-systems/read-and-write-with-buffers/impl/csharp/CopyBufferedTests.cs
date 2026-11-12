// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-with-buffers/.

using Xunit;

public sealed class CopyBufferedTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("copy-buffered-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    private static readonly byte[] Text = "alpha\nbeta\ngamma\n"u8.ToArray();

    private static byte[] Pattern(int n) => Enumerable.Range(0, n).Select(i => (byte)i).ToArray();

    private static readonly Dictionary<string, (byte[] Data, int Size)> Cases = new()
    {
        ["text"] = (Text, 4096),
        ["several-buffers"] = (Text, 4),
        ["empty"] = ([], 8),
        ["smaller-than-buffer"] = ("12345"u8.ToArray(), 8),
        ["exact-buffer"] = ("12345678"u8.ToArray(), 8),
        ["one-past-buffer"] = ("123456789"u8.ToArray(), 8),
        ["size-one"] = (Text, 1),
        ["binary"] = (Pattern(256), 64),
        ["large"] = (Pattern(1 << 20), 4096),
        ["large-odd-buffer"] = (Pattern(1 << 20), 4093),
    };

    public static TheoryData<string> Names => new(Cases.Keys);

    private string Write(string name, byte[] data)
    {
        string path = Path.Combine(dir, name);
        File.WriteAllBytes(path, data);
        return path;
    }

    [Theory]
    [MemberData(nameof(Names))]
    public void Copies(string name)
    {
        var (data, size) = Cases[name];
        string src = Write("in.bin", data);
        string dst = Path.Combine(dir, "out.bin");
        Assert.Equal(data.Length, CopyBuffered.Copy(src, dst, size));
        Assert.Equal(data, File.ReadAllBytes(dst));
    }

    [Fact]
    public void ReplacesALongerFile()
    {
        string src = Write("in.bin", "new"u8.ToArray());
        string dst = Write("out.bin", "old and longer"u8.ToArray());
        Assert.Equal(3, CopyBuffered.Copy(src, dst, 8));
        Assert.Equal("new"u8.ToArray(), File.ReadAllBytes(dst));
    }

    [Fact]
    public void MissingSrc()
    {
        Assert.Throws<FileNotFoundException>(
            () => CopyBuffered.Copy(Path.Combine(dir, "missing.bin"), Path.Combine(dir, "out.bin"), 8));
        Assert.False(File.Exists(Path.Combine(dir, "out.bin")));
    }

    [Fact]
    public void MissingDstDir()
    {
        string src = Write("in.bin", Text);
        Assert.Throws<DirectoryNotFoundException>(
            () => CopyBuffered.Copy(src, Path.Combine(dir, "no-such-dir", "out.bin"), 8));
    }

    [Theory]
    [InlineData(0)]
    [InlineData(-1)]
    public void InvalidSize(int size)
    {
        string src = Write("in.bin", Text);
        Assert.Throws<ArgumentOutOfRangeException>(() => CopyBuffered.Copy(src, Path.Combine(dir, "out.bin"), size));
    }

    [Theory]
    [InlineData(17)]
    [InlineData(1 << 20)]
    public void FullDevice(int n)
    {
        if (!File.Exists("/dev/full"))
        {
            return;
        }
        // 17 bytes stay in the buffer, so the error comes only at the flush.
        string src = Write("in.bin", Pattern(n));
        Assert.ThrowsAny<IOException>(() => CopyBuffered.Copy(src, "/dev/full", 4096));
    }
}
