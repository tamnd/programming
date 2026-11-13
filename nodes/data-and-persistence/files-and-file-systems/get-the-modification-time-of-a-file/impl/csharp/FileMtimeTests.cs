// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

using Xunit;

public sealed class FileMtimeTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("file-mtime-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    private static DateTime FromNs(long ns) => DateTime.UnixEpoch.AddTicks(ns / 100);

    // name, path, mtime_ns, result
    public static TheoryData<string, string, long, long> Cases => new()
    {
        { "whole-second", "a.txt", 1709296496000000000, 1709296496 },
        { "sub-second", "a.txt", 1709296496500000000, 1709296496 },
        { "epoch", "a.txt", 0, 0 },
        { "before-1970", "a.txt", -86400000000000, -86400 },
        { "before-1970-sub-second", "a.txt", -1500000000, -2 },
        { "after-2038", "a.txt", 4102444800000000000, 4102444800 },
        { "directory", "dir", 1709296496000000000, 1709296496 },
        { "name-with-spaces", "my notes.txt", 1709296496000000000, 1709296496 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Case(string name, string rel, long ns, long want)
    {
        string path = Path.Combine(dir, rel);
        if (rel == "dir")
        {
            Directory.CreateDirectory(path);
            Directory.SetLastWriteTimeUtc(path, FromNs(ns));
        }
        else
        {
            File.WriteAllText(path, "alpha\n");
            File.SetLastWriteTimeUtc(path, FromNs(ns));
        }
        Assert.True(want == FileMtime.Mtime(path), name);
    }

    [Fact]
    public void Errors()
    {
        string path = Path.Combine(dir, "a.txt");
        Assert.Throws<FileNotFoundException>(() => FileMtime.Mtime(path));
        // File.GetLastWriteTimeUtc alone gives a time, not an error.
        Assert.Equal(new DateTime(1601, 1, 1, 0, 0, 0, DateTimeKind.Utc), File.GetLastWriteTimeUtc(path));
    }

    [Fact]
    public void SymbolicLink()
    {
        string target = Path.Combine(dir, "a.txt");
        File.WriteAllText(target, "alpha\n");
        File.SetLastWriteTimeUtc(target, FromNs(1709296496000000000));
        File.CreateSymbolicLink(Path.Combine(dir, "link"), "a.txt");
        Assert.Equal(1709296496, FileMtime.Mtime(Path.Combine(dir, "link")));
        // File.GetLastWriteTimeUtc alone gives the time of the link.
        Assert.NotEqual(FromNs(1709296496000000000), File.GetLastWriteTimeUtc(Path.Combine(dir, "link")));
    }

    [Fact]
    public void WriteSetsThePresentTime()
    {
        string path = Path.Combine(dir, "a.txt");
        File.WriteAllText(path, "alpha\n");
        File.SetLastWriteTimeUtc(path, DateTime.UnixEpoch);
        long before = DateTimeOffset.UtcNow.ToUnixTimeSeconds() - 1;
        File.AppendAllText(path, "beta\n");
        Assert.True(FileMtime.Mtime(path) >= before);
    }
}
