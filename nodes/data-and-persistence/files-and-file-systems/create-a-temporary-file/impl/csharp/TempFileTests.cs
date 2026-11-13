// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-temporary-file/.

using System.Runtime.Versioning;
using Xunit;

[UnsupportedOSPlatform("windows")]
public sealed class TempFileTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("temp-file-").FullName;

    public TempFileTests() => Directory.CreateDirectory(Path.Combine(dir, "sub"));

    public void Dispose() => Directory.Delete(dir, recursive: true);

    public static TheoryData<string, string, string, string> Cases => new()
    {
        { "one-line", ".", "tmp-", "alpha\n" },
        { "many-lines", ".", "report-", "alpha\nbeta\ngamma\n" },
        { "in-subdirectory", "sub", "tmp-", "alpha\n" },
        { "empty-prefix", ".", "", "alpha\n" },
        { "empty-data", ".", "tmp-", "" },
        { "non-ascii-data", ".", "tmp-", "café 日本\n" },
        { "prefix-with-spaces", ".", "my temp ", "alpha\n" },
        { "non-ascii-prefix", ".", "café-", "alpha\n" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void UsesTheFile(string name, string rel, string prefix, string data)
    {
        Assert.Equal(data, TempFile.UseTempFile(Path.Combine(dir, rel), prefix, data));
        Assert.Equal(new[] { "sub" }, Directory.GetFileSystemEntries(dir).Select(Path.GetFileName));
        Assert.Empty(Directory.GetFileSystemEntries(Path.Combine(dir, "sub")));
        Assert.True(name.Length > 0);
    }

    [Theory]
    [InlineData("tmp-")]
    [InlineData("")]
    [InlineData("my temp ")]
    [InlineData("café-")]
    public void MakesANewFile(string prefix)
    {
        string path = TempFile.MakeTempFile(dir, prefix);
        Assert.Equal(dir, Path.GetDirectoryName(path));
        string name = Path.GetFileName(path);
        Assert.StartsWith(prefix, name, StringComparison.Ordinal);
        Assert.True(name.Length >= prefix.Length + 6);
        var info = new FileInfo(path);
        Assert.Null(info.LinkTarget);
        Assert.Equal(UnixFileMode.UserRead | UnixFileMode.UserWrite, File.GetUnixFileMode(path));
        Assert.Equal(0, info.Length);
    }

    [Fact]
    public void TwoCallsGiveTwoFiles()
    {
        string a = TempFile.MakeTempFile(dir, "tmp-");
        string b = TempFile.MakeTempFile(dir, "tmp-");
        Assert.NotEqual(a, b);
        Assert.Equal(3, Directory.GetFileSystemEntries(dir).Length);
    }

    [Fact]
    public void Errors()
    {
        Assert.Throws<DirectoryNotFoundException>(() => TempFile.UseTempFile(Path.Combine(dir, "nodir"), "tmp-", "alpha\n"));
        Assert.Throws<ArgumentException>(() => TempFile.UseTempFile(dir, "sub/tmp-", "alpha\n"));
        Assert.Empty(Directory.GetFileSystemEntries(Path.Combine(dir, "sub")));
    }
}
