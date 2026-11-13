// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/truncate-a-file/.

using System.Text;
using Xunit;

public sealed class TruncateFileTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("truncate-file-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    // name, path, length, content before, content after
    public static TheoryData<string, string, long, string, string> Cases => new()
    {
        { "shrink", "a.txt", 5, "alpha beta\n", "alpha" },
        { "extend", "a.txt", 8, "alpha", "alpha\0\0\0" },
        { "same-length", "a.txt", 6, "alpha\n", "alpha\n" },
        { "to-zero", "a.txt", 0, "alpha\n", "" },
        { "empty-file", "a.txt", 3, "", "\0\0\0" },
        { "name-with-spaces", "my notes.txt", 2, "alpha\n", "al" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Truncates(string name, string rel, long length, string before, string after)
    {
        string path = Path.Combine(dir, rel);
        File.WriteAllBytes(path, Encoding.UTF8.GetBytes(before));
        TruncateFile.Truncate(path, length);
        Assert.True(Encoding.UTF8.GetBytes(after).AsSpan().SequenceEqual(File.ReadAllBytes(path)), name);
    }

    [Fact]
    public void Errors()
    {
        string path = Path.Combine(dir, "a.txt");
        Assert.Throws<FileNotFoundException>(() => TruncateFile.Truncate(path, 0));
        Assert.False(File.Exists(path));
        string sub = Directory.CreateDirectory(Path.Combine(dir, "dir")).FullName;
        Assert.Throws<IOException>(() => TruncateFile.Truncate(sub, 0));
        File.WriteAllText(path, "alpha\n");
        Assert.Throws<ArgumentOutOfRangeException>(() => TruncateFile.Truncate(path, -1));
        Assert.Equal("alpha\n", File.ReadAllText(path));
    }

    // FileMode.Append does not open the file with O_APPEND: the stream seeks
    // to the end at the open and then writes at its own position. So after a
    // truncate to 0, the next write is at the old end, after a hole.
    [Fact]
    public void OpenInAppendMode()
    {
        string path = Path.Combine(dir, "app.log");
        File.WriteAllText(path, "alpha beta\n");
        using (var log = new FileStream(path, FileMode.Append, FileAccess.Write, FileShare.ReadWrite))
        {
            TruncateFile.Truncate(path, 0);
            log.Write("new\n"u8);
        }
        Assert.Equal(new string('\0', 11) + "new\n", File.ReadAllText(path));
    }
}
