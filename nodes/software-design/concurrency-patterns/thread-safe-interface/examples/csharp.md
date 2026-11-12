```csharp run id=interface
using System;
using System.Collections.Generic;
using System.Threading;

class FileCache
{
    private readonly object gate = new object();
    public readonly Dictionary<string, string> Files = new();
    public int Acquires;

    // Interface methods: take the lock, then call an implementation method.
    public string Find(string path)
    {
        lock (gate)
        {
            Acquires++;
            return FindI(path);
        }
    }

    public void Bind(string path)
    {
        lock (gate)
        {
            Acquires++;
            BindI(path);
        }
    }

    // Implementation methods: trust that the lock is held, never take it.
    private string FindI(string path)
    {
        var file = CheckCacheI(path);
        if (file == null)
        {
            BindI(path); // not Bind(): that would take the lock again
            file = CheckCacheI(path);
        }
        return file!;
    }

    private string? CheckCacheI(string path) => Files.TryGetValue(path, out var file) ? file : null;

    private void BindI(string path) => Files[path] = "mapped " + path;
}

static class Program
{
    static void Main()
    {
        var cache = new FileCache();
        cache.Bind("/index.html");
        var paths = new[] { "/a.html", "/b.html", "/c.html" };
        var threads = new List<Thread>();
        for (int w = 0; w < 4; w++)
        {
            var t = new Thread(() =>
            {
                for (int i = 0; i < 1000; i++) cache.Find(paths[i % 3]);
            });
            threads.Add(t);
            t.Start();
        }
        foreach (var t in threads) t.Join();
        Console.WriteLine("find /a.html: " + cache.Find("/a.html"));
        Console.WriteLine("files cached: " + cache.Files.Count);
        Console.WriteLine("lock acquisitions: " + cache.Acquires);
    }
}
```

```output
find /a.html: mapped /a.html
files cached: 4
lock acquisitions: 4002
```

- Verdict: idiom. Public methods take the `lock` and private methods do the work, so each interface call takes the lock once.
