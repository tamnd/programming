// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/set-file-permissions/.

using System.Runtime.Versioning;
using System.Text.RegularExpressions;
using Xunit;

[UnsupportedOSPlatform("windows")]
public sealed class SetModeTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("set-mode-").FullName;

    public void Dispose()
    {
        foreach (var f in Directory.EnumerateFileSystemEntries(dir))
        {
            if (!Directory.Exists(f)) File.SetUnixFileMode(f, UnixFileMode.UserRead | UnixFileMode.UserWrite);
        }
        Directory.Delete(dir, recursive: true);
    }

    private string NewFile(string name)
    {
        var p = Path.Combine(dir, name);
        File.WriteAllText(p, "alpha\n");
        File.SetUnixFileMode(p, (UnixFileMode)Convert.ToInt32("644", 8));
        return p;
    }

    // name, path, mode, result
    public static TheoryData<string, string, string, int> Cases => new()
    {
        { "owner-read-write-group-read", "a.txt", "640", 416 },
        { "owner-only", "a.txt", "600", 384 },
        { "executable", "a.txt", "755", 493 },
        { "same-mode", "a.txt", "644", 420 },
        { "no-permissions", "a.txt", "000", 0 },
        { "leading-zero", "a.txt", "0640", 416 },
        { "set-user-id", "a.txt", "4755", 2541 },
        { "name-with-spaces", "my notes.txt", "640", 416 },
        { "non-ascii-name", "café 日本.txt", "640", 416 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void SetsTheMode(string name, string rel, string mode, int result)
    {
        Assert.NotEmpty(name);
        var p = NewFile(rel);
        Assert.Equal(result, SetMode.Set(p, mode));
        Assert.Equal(result, (int)File.GetUnixFileMode(p));
    }

    [Theory]
    [InlineData("648")]
    [InlineData("64")]
    [InlineData("10640")]
    [InlineData("u+x")]
    [InlineData("")]
    [InlineData("640\n")]
    public void RejectsAnInvalidMode(string mode)
    {
        var p = NewFile("a.txt");
        Assert.Throws<ArgumentException>(() => SetMode.Set(p, mode));
        Assert.Equal(420, (int)File.GetUnixFileMode(p));
    }

    [Fact]
    public void TheDollarSignOfDotNetMatchesBeforeANewline()
    {
        Assert.Matches(new Regex("^[0-7]{3,4}$"), "640\n");
    }

    [Fact]
    public void MissingFile()
    {
        Assert.Throws<FileNotFoundException>(() => SetMode.Set(Path.Combine(dir, "missing.txt"), "640"));
    }

    [Fact]
    public void ASymbolicLinkChangesTheTarget()
    {
        var target = NewFile("a.txt");
        var link = Path.Combine(dir, "link");
        File.CreateSymbolicLink(link, "a.txt");
        Assert.Equal(384, SetMode.Set(link, "600"));
        Assert.Equal(384, (int)File.GetUnixFileMode(target));
    }

    [Fact]
    public void ADirectory()
    {
        var d = Directory.CreateDirectory(Path.Combine(dir, "dir")).FullName;
        Assert.Equal(448, SetMode.Set(d, "700"));
    }
}
