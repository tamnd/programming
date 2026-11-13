// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-a-file/.

using System.Text;
using Xunit;

public sealed class CopyFileTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("copy-file-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    // name, src, dst, content of src, content of dst or null
    public static TheoryData<string, string, string, string, string?> Cases => new()
    {
        { "copy", "a.txt", "b.txt", "alpha\n", null },
        { "replace-existing", "a.txt", "b.txt", "alpha\n", "beta, a longer text\n" },
        { "empty-file", "a.txt", "b.txt", "", "beta\n" },
        { "other-directory", "a.txt", "sub/a.txt", "alpha\n", null },
        { "name-with-spaces", "my notes.txt", "copy of my notes.txt", "alpha\n", null },
        { "non-ascii-name", "a.txt", "café 日本.txt", "alpha\n", null },
        { "binary-data", "a.bin", "b.bin", "\0\r\nÿ\0", null },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Copies(string name, string srcRel, string dstRel, string srcText, string? dstText)
    {
        string src = Path.Combine(dir, srcRel), dst = Path.Combine(dir, dstRel);
        Directory.CreateDirectory(Path.GetDirectoryName(dst)!);
        byte[] want = Encoding.UTF8.GetBytes(srcText);
        if (dstText != null)
        {
            File.WriteAllBytes(dst, Encoding.UTF8.GetBytes(dstText));
        }
        File.WriteAllBytes(src, want);
        CopyFile.Copy(src, dst);
        Assert.True(want.AsSpan().SequenceEqual(File.ReadAllBytes(dst)), name);
        Assert.True(want.AsSpan().SequenceEqual(File.ReadAllBytes(src)), name);
    }

    [Fact]
    public void Missing()
    {
        string src = Path.Combine(dir, "a.txt");
        Assert.Throws<FileNotFoundException>(() => CopyFile.Copy(src, Path.Combine(dir, "b.txt")));
        File.WriteAllText(src, "alpha\n");
        Assert.Throws<DirectoryNotFoundException>(() => CopyFile.Copy(src, Path.Combine(dir, "no-such-dir", "b.txt")));
    }

    [Fact]
    public void Directories()
    {
        string sub = Directory.CreateDirectory(Path.Combine(dir, "dir")).FullName;
        string src = Path.Combine(dir, "a.txt");
        File.WriteAllText(src, "alpha\n");
        Assert.Throws<IOException>(() => CopyFile.Copy(sub, Path.Combine(dir, "b.txt")));
        Assert.Throws<IOException>(() => CopyFile.Copy(src, sub));
        Assert.Empty(Directory.GetFileSystemEntries(sub));
        Assert.False(File.Exists(Path.Combine(dir, "b.txt")));
    }

    [Fact]
    public void SamePath()
    {
        string src = Path.Combine(dir, "a.txt");
        File.WriteAllText(src, "alpha\n");
        Assert.Throws<IOException>(() => CopyFile.Copy(src, src));
        Assert.Equal("alpha\n", File.ReadAllText(src));
    }

    [Fact]
    public void HardLink()
    {
        if (OperatingSystem.IsWindows())
        {
            return;
        }
        string src = Path.Combine(dir, "a.txt"), link = Path.Combine(dir, "b.txt");
        File.WriteAllText(src, "alpha\n");
        using (var ln = System.Diagnostics.Process.Start("ln", new[] { src, link }))
        {
            ln.WaitForExit();
            Assert.Equal(0, ln.ExitCode);
        }
        Assert.Throws<IOException>(() => CopyFile.Copy(src, link));
        Assert.Equal("alpha\n", File.ReadAllText(src));
    }

    [Fact]
    public void Mode()
    {
        if (OperatingSystem.IsWindows())
        {
            return;
        }
        const UnixFileMode bits = UnixFileMode.UserRead | UnixFileMode.UserWrite | UnixFileMode.UserExecute
            | UnixFileMode.GroupRead | UnixFileMode.GroupExecute;
        string src = Path.Combine(dir, "a.sh"), other = Path.Combine(dir, "c.sh");
        File.WriteAllText(src, "echo alpha\n");
        File.SetUnixFileMode(src, bits);
        File.WriteAllText(other, "beta\n");
        File.SetUnixFileMode(other, UnixFileMode.UserRead | UnixFileMode.UserWrite | UnixFileMode.GroupRead);
        foreach (string dst in new[] { Path.Combine(dir, "b.sh"), other })
        {
            CopyFile.Copy(src, dst);
            Assert.Equal(bits, File.GetUnixFileMode(dst));
        }
    }

    [Fact]
    public void LargeFile()
    {
        byte[] data = new byte[5_000_003];
        for (int i = 0; i < data.Length; i++)
        {
            data[i] = (byte)((long)i * 7 % 251);
        }
        string src = Path.Combine(dir, "big.bin"), dst = Path.Combine(dir, "copy.bin");
        File.WriteAllBytes(src, data);
        CopyFile.Copy(src, dst);
        Assert.True(data.AsSpan().SequenceEqual(File.ReadAllBytes(dst)));
    }

    [Fact]
    public void LinkAtSrc()
    {
        File.WriteAllText(Path.Combine(dir, "target.txt"), "alpha\n");
        File.CreateSymbolicLink(Path.Combine(dir, "link.txt"), "target.txt");
        string dst = Path.Combine(dir, "b.txt");
        CopyFile.Copy(Path.Combine(dir, "link.txt"), dst);
        Assert.Null(new FileInfo(dst).LinkTarget);
        Assert.Equal("alpha\n", File.ReadAllText(dst));
    }
}
