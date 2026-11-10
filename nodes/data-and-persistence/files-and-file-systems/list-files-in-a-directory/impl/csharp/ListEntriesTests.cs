// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/list-files-in-a-directory/.

using Xunit;

public sealed class ListEntriesTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("list-entries-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    private void MakeTree(string[] tree)
    {
        foreach (string entry in tree)
        {
            int arrow = entry.IndexOf(" -> ", StringComparison.Ordinal);
            if (arrow >= 0)
            {
                File.CreateSymbolicLink(Path.Combine(dir, entry[..arrow]), entry[(arrow + 4)..]);
            }
            else if (entry.EndsWith('/'))
            {
                Directory.CreateDirectory(Path.Combine(dir, entry));
            }
            else
            {
                File.WriteAllBytes(Path.Combine(dir, entry), []);
            }
        }
    }

    private static readonly string[] Tree = ["docs/", "docs/guide.md", "docs/img/", "docs/img/logo.png", "readme.md"];

    public static TheoryData<string, string[], bool, string[]> Cases => new()
    {
        { "flat", ["b.txt", "a.txt", "c.txt"], false, ["a.txt", "b.txt", "c.txt"] },
        { "nested", Tree, true, Tree },
        { "nested-shallow", Tree, false, ["docs/", "readme.md"] },
        { "empty", [], true, [] },
        { "empty-subdir", ["empty/"], true, ["empty/"] },
        { "hidden", [".env", ".git/", ".git/config", "main.go"], true, [".env", ".git/", ".git/config", "main.go"] },
        { "case-order", ["a.txt", "B.txt", "c.txt"], false, ["B.txt", "a.txt", "c.txt"] },
        { "dot-before-slash", ["a/", "a/x", "a.txt", "a-b"], true, ["a-b", "a.txt", "a/", "a/x"] },
        { "symlink-to-dir", ["real/", "real/f", "link -> real"], true, ["link", "real/", "real/f"] },
        { "symlink-to-file", ["f", "g -> f"], true, ["f", "g"] },
        { "broken-symlink", ["dangling -> nowhere"], true, ["dangling"] },
        { "symlink-loop", ["d/", "d/up -> .."], true, ["d/", "d/up"] },
        { "spaces", ["my file.txt", "two  spaces/", "two  spaces/x"], true, ["my file.txt", "two  spaces/", "two  spaces/x"] },
        { "non-ascii", ["été.txt", "zebra.txt", "apple.txt"], false, ["apple.txt", "zebra.txt", "été.txt"] },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Lists(string name, string[] tree, bool recursive, string[] want)
    {
        Assert.False(string.IsNullOrEmpty(name));
        MakeTree(tree);
        Assert.Equal(want, ListEntries.List(dir, recursive));
    }

    [Fact]
    public void Missing() =>
        Assert.Throws<DirectoryNotFoundException>(() => ListEntries.List(Path.Combine(dir, "missing"), true));

    [Fact]
    public void NotADirectory()
    {
        MakeTree(["notes.txt"]);
        var e = Assert.ThrowsAny<IOException>(() => ListEntries.List(Path.Combine(dir, "notes.txt"), true));
        Assert.IsNotType<DirectoryNotFoundException>(e);
    }
}
