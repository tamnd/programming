// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-symbolic-link/.

using Xunit;

public sealed class MakeSymlinkTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("make-symlink-").FullName;

    public MakeSymlinkTests()
    {
        File.WriteAllText(Path.Combine(dir, "a.txt"), "alpha\n");
        Directory.CreateDirectory(Path.Combine(dir, "sub"));
        File.WriteAllText(Path.Combine(dir, "sub", "b.txt"), "beta\n");
    }

    public void Dispose() => Directory.Delete(dir, recursive: true);

    public static TheoryData<string, string, string> Cases => new()
    {
        { "file-target", "a.txt", "link" },
        { "directory-target", "sub", "link" },
        { "link-in-subdirectory", "b.txt", "sub/link" },
        { "dangling-target", "missing.txt", "link" },
        { "relative-target", "a.txt", "sub/link" },
        { "parent-target", "../a.txt", "sub/link" },
        { "name-with-spaces", "my notes.txt", "my link" },
        { "non-ascii-name", "café 日本.txt", "lien 日本" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void MakesTheLink(string name, string target, string rel)
    {
        string link = Path.Combine(dir, rel);
        Assert.Equal(target, MakeSymlink.Make(target, link));
        Assert.Equal(target, new FileInfo(link).LinkTarget);
        Assert.True(name.Length > 0);
    }

    [Theory]
    [InlineData("b.txt", "a.txt")]
    [InlineData("a.txt", "sub")]
    public void ExistingPathIsAnError(string target, string rel)
    {
        Assert.ThrowsAny<IOException>(() => MakeSymlink.Make(target, Path.Combine(dir, rel)));
        Assert.Null(new FileInfo(Path.Combine(dir, rel)).LinkTarget);
    }

    [Fact]
    public void DanglingLinkExists()
    {
        string link = Path.Combine(dir, "link");
        File.CreateSymbolicLink(link, "missing.txt");
        Assert.ThrowsAny<IOException>(() => MakeSymlink.Make("a.txt", link));
        Assert.Equal("missing.txt", new FileInfo(link).LinkTarget);
    }

    [Fact]
    public void MissingDirectory()
    {
        Assert.Throws<DirectoryNotFoundException>(
            () => MakeSymlink.Make("a.txt", Path.Combine(dir, "nodir", "link")));
    }

    [Fact]
    public void LinkReadsTarget()
    {
        MakeSymlink.Make("a.txt", Path.Combine(dir, "link"));
        Assert.Equal("alpha\n", File.ReadAllText(Path.Combine(dir, "link")));
        MakeSymlink.Make("../a.txt", Path.Combine(dir, "sub", "up"));
        Assert.Equal("alpha\n", File.ReadAllText(Path.Combine(dir, "sub", "up")));
    }

    [Fact]
    public void AbsoluteTarget()
    {
        string target = Path.Combine(dir, "a.txt");
        Assert.Equal(target, MakeSymlink.Make(target, Path.Combine(dir, "link")));
    }
}
