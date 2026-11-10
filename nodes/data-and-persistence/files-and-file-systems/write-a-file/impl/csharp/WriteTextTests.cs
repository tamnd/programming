// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-file/.

using System.Text;
using Xunit;

public sealed class WriteTextTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("write-text-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    private string[] Names() => Directory.GetFileSystemEntries(dir).Select(Path.GetFileName).ToArray()!;

    public static TheoryData<string, string, WriteMode, string?, string> Cases => new()
    {
        { "new-file", "alpha\nbeta\n", WriteMode.Truncate, null, "alpha\nbeta\n" },
        { "append-existing", "two\n", WriteMode.Append, "one\n", "one\ntwo\n" },
        { "atomic-replace", "new\n", WriteMode.Atomic, "old\n", "new\n" },
        { "truncate-longer", "short\n", WriteMode.Truncate, "a much longer old line\n", "short\n" },
        { "empty-truncate", "", WriteMode.Truncate, "old\n", "" },
        { "empty-new", "", WriteMode.Truncate, null, "" },
        { "append-missing", "first\n", WriteMode.Append, null, "first\n" },
        { "append-no-newline", "two", WriteMode.Append, "one", "onetwo" },
        { "non-ascii", "naïve café\n日本語\n🙂\n", WriteMode.Truncate, null, "naïve café\n日本語\n🙂\n" },
        { "crlf", "one\r\ntwo\r\n", WriteMode.Truncate, null, "one\r\ntwo\r\n" },
        { "mixed-endings", "lf\ncrlf\r\ncr\rend", WriteMode.Truncate, null, "lf\ncrlf\r\ncr\rend" },
        { "backslashes", "C:\\temp\\new\n", WriteMode.Truncate, null, "C:\\temp\\new\n" },
        { "format-chars", "-n 100% %s\n", WriteMode.Truncate, null, "-n 100% %s\n" },
        { "atomic-new", "new\n", WriteMode.Atomic, null, "new\n" },
        { "atomic-empty", "", WriteMode.Atomic, "old\n", "" },
        { "atomic-non-ascii", "naïve café\n日本語\n🙂\n", WriteMode.Atomic, "old\n", "naïve café\n日本語\n🙂\n" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void WritesTheBytes(string name, string text, WriteMode mode, string? before, string want)
    {
        string path = Path.Combine(dir, "out.txt");
        if (before is not null)
        {
            File.WriteAllBytes(path, Encoding.UTF8.GetBytes(before));
        }
        WriteFile.WriteText(path, text, mode);
        // The exact bytes: UTF-8, no byte order mark, no change to the line ends.
        Assert.True(Encoding.UTF8.GetBytes(want).SequenceEqual(File.ReadAllBytes(path)), name);
        Assert.Equal(["out.txt"], Names());
    }

    [Theory]
    [InlineData(WriteMode.Truncate)]
    [InlineData(WriteMode.Append)]
    [InlineData(WriteMode.Atomic)]
    public void MissingDirectory(WriteMode mode)
    {
        string path = Path.Combine(dir, "missing", "out.txt");
        Assert.Throws<DirectoryNotFoundException>(() => WriteFile.WriteText(path, "x\n", mode));
        Assert.Empty(Names());
    }

    [Fact]
    public void AtomicOntoDirectory()
    {
        string path = Directory.CreateDirectory(Path.Combine(dir, "out.txt")).FullName;
        Assert.ThrowsAny<IOException>(() => WriteFile.WriteText(path, "x\n", WriteMode.Atomic));
        Assert.True(Directory.Exists(path));
        Assert.Equal(["out.txt"], Names());
    }
}
