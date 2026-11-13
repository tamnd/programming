// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-size-of-a-file/.

using System.Text;
using Xunit;

public sealed class FileSizeTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("file-size-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    // name, path, content, size
    public static TheoryData<string, string, string, long> Cases => new()
    {
        { "text", "a.txt", "alpha\n", 6 },
        { "non-ascii-text", "a.txt", "café\n", 6 },
        { "empty-file", "a.txt", "", 0 },
        { "binary-data", "a.bin", "\0\r\nÿ\0", 6 },
        { "crlf-line-ends", "a.txt", "a\r\nb\r\n", 6 },
        { "name-with-spaces", "my notes.txt", "alpha\n", 6 },
        { "non-ascii-name", "café 日本.txt", "alpha\n", 6 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Case(string name, string rel, string data, long size)
    {
        string path = Path.Combine(dir, rel);
        File.WriteAllBytes(path, Encoding.UTF8.GetBytes(data));
        Assert.True(size == FileSize.Size(path), name);
    }

    [Fact]
    public void Errors()
    {
        Assert.Throws<FileNotFoundException>(() => FileSize.Size(Path.Combine(dir, "a.txt")));
        Directory.CreateDirectory(Path.Combine(dir, "dir"));
        var e = Assert.Throws<IOException>(() => FileSize.Size(Path.Combine(dir, "dir")));
        Assert.Contains("Is a directory", e.Message);
    }

    [Fact]
    public void SymbolicLink()
    {
        File.WriteAllText(Path.Combine(dir, "a.txt"), "alpha beta\n");
        File.CreateSymbolicLink(Path.Combine(dir, "link"), "a.txt");
        Assert.Equal(11, FileSize.Size(Path.Combine(dir, "link")));
        // FileInfo.Length alone gives the size of the link: the 5 bytes of "a.txt".
        Assert.Equal(5, new FileInfo(Path.Combine(dir, "link")).Length);
    }

    [Fact]
    public void SparseFileOf5GiB()
    {
        string path = Path.Combine(dir, "sparse.bin");
        using (var f = new FileStream(path, FileMode.CreateNew, FileAccess.Write))
        {
            f.SetLength(5L * 1024 * 1024 * 1024);
        }
        Assert.Equal(5368709120L, FileSize.Size(path));
    }

    [Fact]
    public void OpenFileAfterFlush()
    {
        string path = Path.Combine(dir, "app.log");
        using var f = new FileStream(path, FileMode.CreateNew, FileAccess.Write);
        f.Write("alpha\n"u8);
        f.Flush();
        Assert.Equal(6, FileSize.Size(path));
    }
}
