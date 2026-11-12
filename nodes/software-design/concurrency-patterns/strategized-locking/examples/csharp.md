```csharp run id=strategy
using System;
using System.Collections.Generic;
using System.Threading;

interface ILockable
{
    void Acquire();
    void Release();
}

// A lock for one thread: both methods do nothing.
class NullMutex : ILockable
{
    public void Acquire() { }
    public void Release() { }
}

class ThreadMutex : ILockable
{
    private readonly object gate = new object();
    public void Acquire() => Monitor.Enter(gate);
    public void Release() => Monitor.Exit(gate);
}

class FileCache
{
    private readonly ILockable lockable; // the locking strategy is a parameter
    public readonly Dictionary<string, string> Files = new();
    public int Lookups;

    public FileCache(ILockable lockable) => this.lockable = lockable;

    public string Find(string path)
    {
        lockable.Acquire();
        try
        {
            Lookups++;
            if (!Files.TryGetValue(path, out var file))
            {
                file = "mapped " + path;
                Files[path] = file;
            }
            return file;
        }
        finally
        {
            lockable.Release();
        }
    }
}

static class Program
{
    static void Main()
    {
        var single = new FileCache(new NullMutex());
        foreach (var path in new[] { "/a.html", "/b.html", "/a.html" }) single.Find(path);
        Console.WriteLine($"Null_Mutex cache: {single.Files.Count} files, {single.Lookups} lookups");

        var multi = new FileCache(new ThreadMutex());
        var paths = new[] { "/a.html", "/b.html", "/c.html" };
        var threads = new List<Thread>();
        for (int w = 0; w < 4; w++)
        {
            var t = new Thread(() =>
            {
                for (int i = 0; i < 1000; i++) multi.Find(paths[i % 3]);
            });
            threads.Add(t);
            t.Start();
        }
        foreach (var t in threads) t.Join();
        Console.WriteLine($"Thread_Mutex cache: {multi.Files.Count} files, {multi.Lookups} lookups");
    }
}
```

```output
Null_Mutex cache: 2 files, 3 lookups
Thread_Mutex cache: 3 files, 4000 lookups
```

- Verdict: idiom. This is the polymorphic form. `FileCache` sees only `ILockable`, and `ThreadMutex` adapts `Monitor` to that interface.
