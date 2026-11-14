// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-text-in-many-files/.

using System.Text;
using Xunit;

public sealed class ReplaceInFilesTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("replace-in-files-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    // Each file is (name, text before, text after).
    public static TheoryData<string, string, string, (string, string, string)[], int[]> Cases => new()
    {
        { "rosetta", "Goodbye London!", "Hello New York!", [("a.txt", "Goodbye London!\n", "Hello New York!\n"),
            ("b.txt", "Goodbye London! Goodbye London!\nGoodbye\n", "Hello New York! Hello New York!\nGoodbye\n")], [1, 2] },
        { "some-files-match", "dog", "fox", [("a.txt", "cat\n", "cat\n"), ("b.txt", "dog\ncat dog\n", "fox\ncat fox\n")], [0, 2] },
        { "one-file", "1", "2", [("a.txt", "x = 1\ny = 1\n", "x = 2\ny = 2\n")], [2] },
        { "no-files", "a", "b", [], [] },
        { "no-match", "x", "y", [("a.txt", "abc\n", "abc\n")], [0] },
        { "empty-file", "a", "b", [("a.txt", "", "")], [0] },
        { "delete", "-", "", [("a.txt", "a-b-c\n", "abc\n")], [2] },
        { "whole-file", "abc", "", [("a.txt", "abc", "")], [1] },
        { "new-contains-old", "a", "aa", [("a.txt", "a a\n", "aa aa\n")], [2] },
        { "overlapping", "aa", "b", [("a.txt", "aaa\n", "ba\n")], [1] },
        { "adjacent", "aa", "b", [("a.txt", "aaaa\n", "bb\n")], [2] },
        { "special-chars", "1.5", "\\&$1 $& [x]", [("a.txt", "1.5 and 125\n", "\\&$1 $& [x] and 125\n")], [1] },
        { "glob-chars", "*.txt", "?", [("a.txt", "a.txt *.txt\n", "a.txt ?\n")], [1] },
        { "across-lines", "end\nstart", "end start", [("a.txt", "end\nstart\n", "end start\n")], [1] },
        { "lf-to-crlf", "\n", "\r\n", [("a.txt", "a\nb\n", "a\r\nb\r\n")], [2] },
        { "crlf", "a", "c", [("a.txt", "a\r\nb\r\n", "c\r\nb\r\n")], [1] },
        { "no-final-newline", "b", "c", [("a.txt", "a b", "a c")], [1] },
        { "non-ascii", "\u00e9", "e", [("a.txt", "caf\u00e9, cafe\n", "cafe, cafe\n")], [1] },
        { "case-sensitive", "hello", "bye", [("a.txt", "Hello hello\n", "Hello bye\n")], [1] },
    };

    // UTF8Encoding(false) writes no byte order mark.
    private static void Write(string path, string text) => File.WriteAllText(path, text, new UTF8Encoding(false));

    [Theory]
    [MemberData(nameof(Cases))]
    public void Replace(string name, string oldText, string newText, (string, string, string)[] files, int[] want)
    {
        var paths = files.Select(f => Path.Combine(dir, f.Item1)).ToArray();
        for (int i = 0; i < files.Length; i++)
        {
            Write(paths[i], files[i].Item2);
        }
        Assert.Equal(want, ReplaceInFiles.Replace(paths, oldText, newText));
        for (int i = 0; i < files.Length; i++)
        {
            Assert.Equal(files[i].Item3, File.ReadAllText(paths[i], Encoding.UTF8));
        }
        // No temporary file stays in the directory.
        Assert.Equal(files.Length, Directory.GetFiles(dir).Length);
        _ = name;
    }

    private static string Lines(string prefix) =>
        string.Concat(Enumerable.Range(0, 100000).Select(i => $"{prefix}{i}\n"));

    [Fact]
    public void ManyLines()
    {
        string path = Path.Combine(dir, "many.txt");
        Write(path, Lines("line "));
        Assert.Equal([100000], ReplaceInFiles.Replace([path], "line ", "L"));
        Assert.Equal(Lines("L"), File.ReadAllText(path));
    }

    [Fact]
    public void KeepsThePermissions()
    {
        if (OperatingSystem.IsWindows())
        {
            return;
        }
        string path = Path.Combine(dir, "a.txt");
        Write(path, "a\n");
        var mode = UnixFileMode.UserRead | UnixFileMode.UserWrite | UnixFileMode.GroupRead;
        File.SetUnixFileMode(path, mode);
        Assert.Equal([1], ReplaceInFiles.Replace([path], "a", "b"));
        Assert.Equal(mode, File.GetUnixFileMode(path));
    }

    [Fact]
    public void NotFound()
    {
        Assert.Throws<FileNotFoundException>(() => ReplaceInFiles.Replace([Path.Combine(dir, "missing.txt")], "a", "b"));
    }

    [Fact]
    public void EmptyOld()
    {
        string path = Path.Combine(dir, "a.txt");
        Write(path, "a\n");
        Assert.Throws<ArgumentException>(() => ReplaceInFiles.Replace([path], "", "b"));
        Assert.Equal("a\n", File.ReadAllText(path));
    }

    [Fact]
    public void Stop()
    {
        string a = Path.Combine(dir, "a.txt"), c = Path.Combine(dir, "c.txt");
        Write(a, "a\n");
        Write(c, "a\n");
        Assert.Throws<FileNotFoundException>(() => ReplaceInFiles.Replace([a, Path.Combine(dir, "missing.txt"), c], "a", "b"));
        Assert.Equal("b\n", File.ReadAllText(a));
        Assert.Equal("a\n", File.ReadAllText(c));
    }
}
