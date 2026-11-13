// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sync-a-file-to-disk/.

using System.Runtime.Versioning;
using System.Text;
using Xunit;

[UnsupportedOSPlatform("windows")]
public sealed class WriteSyncedTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("sync-file-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    public static TheoryData<string, string, string?> Cases => new()
    {
        { "new-file", "alpha\nbeta\n", null },
        { "replace-longer", "short\n", "a much longer old line\n" },
        { "many-lines", "one\ntwo\nthree\nfour\n", null },
        { "empty-replace", "", "old\n" },
        { "empty-new", "", null },
        { "no-final-newline", "last line", null },
        { "non-ascii", "naïve café\n日本語\n🙂\n", null },
        { "crlf", "one\r\ntwo\r\n", null },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Writes(string name, string text, string? before)
    {
        string path = Path.Combine(dir, "out.txt");
        if (before != null)
        {
            File.WriteAllText(path, before);
        }
        WriteSynced.Write(path, text);
        Assert.Equal(Encoding.UTF8.GetBytes(text), File.ReadAllBytes(path));
        Assert.Equal(new[] { "out.txt" }, Directory.GetFileSystemEntries(dir).Select(Path.GetFileName));
        Assert.True(name.Length > 0);
    }

    [Fact]
    public void SyncsADirectory() => WriteSynced.SyncDirectory(dir);

    [Fact]
    public void MissingDirectory()
    {
        Assert.Throws<DirectoryNotFoundException>(() => WriteSynced.Write(Path.Combine(dir, "missing", "out.txt"), "x\n"));
        Assert.Empty(Directory.GetFileSystemEntries(dir));
    }

    [Fact]
    public void OntoDirectory()
    {
        string path = Path.Combine(dir, "out.txt");
        Directory.CreateDirectory(path);
        Assert.Throws<UnauthorizedAccessException>(() => WriteSynced.Write(path, "x\n"));
        Assert.True(Directory.Exists(path));
    }
}
