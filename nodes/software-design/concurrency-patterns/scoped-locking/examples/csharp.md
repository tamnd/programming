```csharp run id=guard
using System;
using System.Collections.Generic;
using System.Threading;

static class Program
{
    static readonly object Gate = new();
    static readonly Dictionary<string, int> Table = new();

    static int Increment(string path)
    {
        lock (Gate) // released on every way out
        {
            if (path.Length == 0) return -1;
            Table[path] = Table.GetValueOrDefault(path) + 1;
            return 0;
        }
    }

    static void Main()
    {
        var threads = new Thread[4];
        for (int i = 0; i < 4; i++)
        {
            threads[i] = new Thread(() => { for (int j = 0; j < 1000; j++) Increment("/index"); });
            threads[i].Start();
        }
        foreach (var t in threads) t.Join();
        Console.WriteLine($"hits /index: {Table["/index"]}");
        Console.WriteLine($"bad path returns {Increment("")}");
        Console.WriteLine($"lock free after the early return: {!Monitor.IsEntered(Gate)}");
        Increment("/index");
        Console.WriteLine($"hits /index: {Table["/index"]}");
    }
}
```

```output
hits /index: 4000
bad path returns -1
lock free after the early return: True
hits /index: 4001
```

- Verdict: native. The `lock` statement is the idiom in the language. `Monitor.IsEntered` checks that the current thread no longer holds the lock.
