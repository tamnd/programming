// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/lock-a-file/.

using System.Runtime.Versioning;
using Xunit;

[UnsupportedOSPlatform("windows")]
public sealed class LockFileTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("lock-file-").FullName;

    private string Data => Path.Combine(dir, "data.txt");

    public LockFileTests() => File.WriteAllText(Data, "alpha\n");

    public void Dispose() => Directory.Delete(dir, recursive: true);

    [Theory]
    [InlineData("no-lock-exclusive", "none", "exclusive", true)]
    [InlineData("no-lock-shared", "none", "shared", true)]
    [InlineData("exclusive-with-exclusive", "exclusive", "exclusive", false)]
    [InlineData("shared-with-shared", "shared", "shared", true)]
    [InlineData("shared-with-exclusive", "shared", "exclusive", false)]
    [InlineData("exclusive-with-shared", "exclusive", "shared", false)]
    public void TryLock(string name, string held, string want, bool expect)
    {
        Assert.Equal(expect, LockFile.TryLock(Data, held, want));
        Assert.Equal("alpha\n", File.ReadAllText(Data));
        Assert.True(name.Length > 0);
    }

    [Fact]
    public void DisposeFreesTheLock()
    {
        FileStream? first = LockFile.TryLockFile(Data, exclusive: true);
        Assert.NotNull(first);
        Assert.Null(LockFile.TryLockFile(Data, exclusive: true));
        first.Dispose();
        using FileStream? again = LockFile.TryLockFile(Data, exclusive: true);
        Assert.NotNull(again);
    }

    [Fact]
    public void MissingFile()
    {
        Assert.Throws<FileNotFoundException>(() => LockFile.TryLockFile(Path.Combine(dir, "missing.txt"), exclusive: true));
        Assert.False(File.Exists(Path.Combine(dir, "missing.txt")));
    }
}
