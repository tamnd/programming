// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/delete-a-file/.

using Xunit;

public sealed class DeleteFileTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("delete-file-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    private static bool LExists(string path) => Path.Exists(path) || new FileInfo(path).LinkTarget != null;

    // name, path, content before the call or null, result
    public static TheoryData<string, string, string?, bool> Cases => new()
    {
        { "delete", "a.txt", "alpha\n", true },
        { "missing", "a.txt", null, false },
        { "empty-file", "a.txt", "", true },
        { "missing-parent", "no-such-dir/a.txt", null, false },
        { "name-with-spaces", "my notes.txt", "alpha\n", true },
        { "non-ascii-name", "café 日本.txt", "alpha\n", true },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Deletes(string name, string rel, string? content, bool want)
    {
        string path = Path.Combine(dir, rel);
        if (content != null)
        {
            File.WriteAllText(path, content);
        }
        Assert.True(want == DeleteFile.Delete(path), name);
        Assert.False(LExists(path), name);
    }

    [Fact]
    public void ThroughAFile()
    {
        File.WriteAllText(Path.Combine(dir, "a.txt"), "alpha\n");
        Assert.False(DeleteFile.Delete(Path.Combine(dir, "a.txt", "b.txt")));
        Assert.True(File.Exists(Path.Combine(dir, "a.txt")));
    }

    [Fact]
    public void Directory_()
    {
        string sub = Directory.CreateDirectory(Path.Combine(dir, "dir")).FullName;
        Assert.Throws<IOException>(() => DeleteFile.Delete(sub));
        Assert.True(Directory.Exists(sub));
    }

    [Fact]
    public void Links()
    {
        File.WriteAllText(Path.Combine(dir, "a.txt"), "alpha\n");
        Directory.CreateDirectory(Path.Combine(dir, "dir"));
        File.CreateSymbolicLink(Path.Combine(dir, "to-file"), "a.txt");
        Directory.CreateSymbolicLink(Path.Combine(dir, "to-dir"), "dir");
        File.CreateSymbolicLink(Path.Combine(dir, "broken"), "missing.txt");
        foreach (string link in new[] { "to-file", "to-dir", "broken" })
        {
            Assert.True(DeleteFile.Delete(Path.Combine(dir, link)), link);
            Assert.False(LExists(Path.Combine(dir, link)), link);
        }
        Assert.Equal("alpha\n", File.ReadAllText(Path.Combine(dir, "a.txt")));
        Assert.True(Directory.Exists(Path.Combine(dir, "dir")));
    }

    [Fact]
    public void ReadOnlyFile()
    {
        string path = Path.Combine(dir, "a.txt");
        File.WriteAllText(path, "alpha\n");
        File.SetAttributes(path, FileAttributes.ReadOnly);
        Assert.True(DeleteFile.Delete(path));
        Assert.False(LExists(path));
    }

    [Fact]
    public void DirectoryWithNoWritePermission()
    {
        if (OperatingSystem.IsWindows() || Environment.IsPrivilegedProcess)
        {
            return; // root can write to each directory
        }
        string locked = Directory.CreateDirectory(Path.Combine(dir, "locked")).FullName;
        string path = Path.Combine(locked, "a.txt");
        File.WriteAllText(path, "alpha\n");
        File.SetUnixFileMode(locked, UnixFileMode.UserRead | UnixFileMode.UserExecute);
        try
        {
            Assert.Throws<UnauthorizedAccessException>(() => DeleteFile.Delete(path));
            Assert.True(File.Exists(path));
        }
        finally
        {
            File.SetUnixFileMode(locked, UnixFileMode.UserRead | UnixFileMode.UserWrite | UnixFileMode.UserExecute);
        }
    }

    [Fact]
    public void OpenFile()
    {
        if (OperatingSystem.IsWindows())
        {
            return; // on Windows a file that is open with no FileShare.Delete cannot be deleted
        }
        string path = Path.Combine(dir, "a.txt");
        File.WriteAllText(path, "alpha\n");
        using var reader = new StreamReader(path);
        Assert.True(DeleteFile.Delete(path));
        Assert.False(LExists(path));
        Assert.Equal("alpha\n", reader.ReadToEnd()); // on Unix the data stays until the close
    }
}
