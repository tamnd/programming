// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/watch-a-file-for-changes/.

using System.Collections.Concurrent;
using System.Diagnostics;
using Xunit;

public sealed class WatchFileTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("watch-file-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    private string Watched => Path.Combine(dir, "watched.txt");

    public static TheoryData<string, string[], string[]> Cases => new()
    {
        { "create", ["create:hello\n"], ["created"] },
        { "create-then-write", ["create:a\n", "write:a much longer line\n"], ["created", "modified"] },
        { "create-then-delete", ["create:a\n", "delete"], ["created", "deleted"] },
        { "append", ["create:a\n", "append:b\n", "append:c\n"], ["created", "modified", "modified"] },
        { "no-change", ["create:a\n", "none", "none"], ["created", "none", "none"] },
        { "same-size", ["create:abc\n", "write:xyz\n"], ["created", "modified"] },
        { "touch-only", ["create:a\n", "touch"], ["created", "modified"] },
        { "replace-by-rename", ["create:old\n", "replace:new\n"], ["created", "modified"] },
        { "delete-and-create", ["create:a\n", "recreate:b\n"], ["created", "modified"] },
        { "never-created", ["none"], ["none"] },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Polls(string name, string[] steps, string[] expect)
    {
        Assert.False(string.IsNullOrEmpty(name));
        Assert.Equal(expect, WatchFile.WatchChanges(Watched, steps));
    }

    [Fact]
    public void DirectoryIsAnError()
    {
        Assert.Throws<IOException>(() => WatchFile.WatchChanges(Watched, ["mkdir"]));
    }

    [Fact]
    public void SameSizeAndSameTimeHideAChange()
    {
        var t = new DateTime(2001, 1, 1, 0, 0, 0, DateTimeKind.Utc);
        File.WriteAllText(Watched, "abc\n");
        File.SetLastWriteTimeUtc(Watched, t);
        var watcher = new WatchFile.FileWatcher(Watched);
        File.WriteAllText(Watched, "xyz\n");
        File.SetLastWriteTimeUtc(Watched, t);
        Assert.Equal("none", watcher.Poll());
        File.WriteAllText(Watched, "xyz!\n");
        Assert.Equal("modified", watcher.Poll());
    }

    [Fact]
    public async Task WatchCallsOnceForABurst()
    {
        var calls = new ConcurrentQueue<string>();
        using var stop = new CancellationTokenSource();
        var task = Task.Run(() => WatchFile.Watch(
            Watched, calls.Enqueue, TimeSpan.FromMilliseconds(10), TimeSpan.FromMilliseconds(500), stop.Token));
        for (int i = 1; i <= 5; i++)
        {
            File.WriteAllText(Watched, new string('x', i));
            await Task.Delay(20);
        }
        var clock = Stopwatch.StartNew();
        while (calls.IsEmpty && clock.Elapsed < TimeSpan.FromSeconds(5))
        {
            await Task.Delay(10);
        }
        await Task.Delay(600);
        stop.Cancel();
        await task;
        Assert.Equal([Watched], calls.ToArray());
    }
}
