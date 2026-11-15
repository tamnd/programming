// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-patch/.

using System.Text;
using Xunit;

public sealed class ApplyPatchTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("apply-a-patch-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    // name, the old text, the patch, the new text
    public static TheoryData<string, string, string, string> Cases => new()
    {
        { "change-a-line", "a\nb\nc\n", "--- old.txt\n+++ new.txt\n@@ -1,3 +1,3 @@\n a\n-b\n+B\n c\n", "a\nB\nc\n" },
        { "two-hunks", "apple\nbanana\ncherry\ndate\nelder\nfig\ngrape\n", "--- old.txt\n+++ new.txt\n@@ -1,3 +1,3 @@\n apple\n-banana\n+blueberry\n cherry\n@@ -6,2 +6,2 @@\n fig\n-grape\n+guava\n", "apple\nblueberry\ncherry\ndate\nelder\nfig\nguava\n" },
        { "git-header", "def main():\n    print(1)\n", "diff --git a/main.py b/main.py\nindex 1234567..89abcde 100644\n--- a/main.py\n+++ b/main.py\n@@ -1,2 +1,2 @@ def main():\n def main():\n-    print(1)\n+    print(2)\n", "def main():\n    print(2)\n" },
        { "add-at-end", "a\n", "@@ -1 +1,2 @@\n a\n+b\n", "a\nb\n" },
        { "old-empty", "", "--- /dev/null\n+++ b.txt\n@@ -0,0 +1,2 @@\n+a\n+b\n", "a\nb\n" },
        { "delete-all", "a\nb\n", "@@ -1,2 +0,0 @@\n-a\n-b\n", "" },
        { "no-count", "a\nb\nc\n", "@@ -2 +2 @@\n-b\n+x\n", "a\nx\nc\n" },
        { "zero-context", "apple\nbanana\ncherry\ndate\nelder\nfig\ngrape\n", "@@ -3 +3 @@\n-cherry\n+coconut\n@@ -6 +5,0 @@\n-fig\n@@ -7,0 +7 @@\n+honeydew\n", "apple\nbanana\ncoconut\ndate\nelder\ngrape\nhoneydew\n" },
        { "insert-at-start", "b\n", "@@ -0,0 +1 @@\n+a\n", "a\nb\n" },
        { "old-no-final-newline", "a\nb", "@@ -1,2 +1,2 @@\n a\n-b\n\\ No newline at end of file\n+b\n", "a\nb\n" },
        { "new-no-final-newline", "a\nb\n", "@@ -1,2 +1,2 @@\n a\n-b\n+b\n\\ No newline at end of file\n", "a\nb" },
        { "empty-context-line", "a\n\nb\n", "@@ -1,3 +1,3 @@\n a\n\n-b\n+c\n", "a\n\nc\n" },
        { "space-context-line", "a\n\nb\n", "@@ -1,3 +1,3 @@\n a\n \n-b\n+c\n", "a\n\nc\n" },
        { "crlf", "a\r\nb\r\n", "@@ -1,2 +1,2 @@\n a\r\n-b\r\n+c\r\n", "a\r\nc\r\n" },
        { "header-like-line", "x\n-- y\n", "--- old.txt\n+++ new.txt\n@@ -1,2 +1 @@\n x\n--- y\n", "x\n" },
        { "non-ascii", "caf\u00e9\n", "@@ -1 +1 @@\n-caf\u00e9\n+na\u00efve\n", "na\u00efve\n" },
    };

    // name, the old text, the patch, the kind of the error
    public static TheoryData<string, string, string, PatchException.Kinds> Errors => new()
    {
        { "mismatch", "a\nb\nc\n", "@@ -1,3 +1,3 @@\n a\n-x\n+y\n c\n", PatchException.Kinds.Mismatch },
        { "malformed", "a\nb\nc\n", "@@ -1,3 +1,3 @@\n a\n-b\n", PatchException.Kinds.Malformed },
        { "no-hunk", "a\n", "no hunk here\n", PatchException.Kinds.Malformed },
        { "bad-mark", "a\n", "@@ -1 +1 @@\n*a\n", PatchException.Kinds.Malformed },
        { "past-the-end", "a\n", "@@ -2 +2 @@\n-a\n+b\n", PatchException.Kinds.Mismatch },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Apply(string name, string oldText, string patch, string want)
    {
        _ = name;
        Assert.Equal(want, ApplyPatch.Apply(oldText, patch));
    }

    [Theory]
    [MemberData(nameof(Errors))]
    public void Error(string name, string oldText, string patch, PatchException.Kinds kind)
    {
        _ = name;
        PatchException e = Assert.Throws<PatchException>(() => ApplyPatch.Apply(oldText, patch));
        Assert.Equal(kind, e.Kind);
    }

    [Fact]
    public void ManyHunks()
    {
        // 1000 lines, and 10 hunks with no context change 10 of them.
        string oldText = string.Concat(Enumerable.Range(0, 1000).Select(i => $"{i}\n"));
        string patch = string.Concat(Enumerable.Range(0, 10).Select(k => k * 100 + 50).Select(i => $"@@ -{i + 1} +{i + 1} @@\n-{i}\n+x{i}\n"));
        string want = string.Concat(Enumerable.Range(0, 1000).Select(i => i % 100 == 50 ? $"x{i}\n" : $"{i}\n"));
        Assert.Equal(want, ApplyPatch.Apply(oldText, patch));
    }

    [Fact]
    public void ApplyFile()
    {
        // UTF8Encoding(false) writes no byte order mark.
        File.WriteAllText(Path.Combine(dir, "old.txt"), "a\r\nb\r\n", new UTF8Encoding(false));
        File.WriteAllText(Path.Combine(dir, "a.patch"), "@@ -1,2 +1,2 @@\n a\r\n-b\r\n+c\r\n", new UTF8Encoding(false));
        Assert.Equal("a\r\nc\r\n", ApplyPatch.ApplyFile(Path.Combine(dir, "old.txt"), Path.Combine(dir, "a.patch")));
    }

    [Fact]
    public void NotFound()
    {
        File.WriteAllText(Path.Combine(dir, "a.patch"), "@@ -1 +1 @@\n-a\n+b\n", new UTF8Encoding(false));
        Assert.Throws<FileNotFoundException>(() => ApplyPatch.ApplyFile(Path.Combine(dir, "missing.txt"), Path.Combine(dir, "a.patch")));
    }
}
