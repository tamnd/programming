```csharp run id=reactor
using System;
using System.Collections.Generic;

static class Program
{
    static readonly Dictionary<string, Action<string, string>> Handlers = new();

    static void Echo(string handle, string data)
    {
        if (data.Length > 0)
        {
            Console.WriteLine($"{handle}: {data}");
        }
        else
        {
            Console.WriteLine($"close {handle}");
            Handlers.Remove(handle);
        }
    }

    static void Main()
    {
        Handlers["listener"] = (handle, newHandle) =>
        {
            Console.WriteLine($"accept {newHandle}");
            Handlers[newHandle] = Echo;
        };
        // ready stands in for select: handle and data pairs in arrival order
        var ready = new (string Handle, string Data)[]
        {
            ("listener", "conn-1"), ("listener", "conn-2"),
            ("conn-1", "hello"), ("conn-2", "hi"),
            ("conn-1", ""), ("conn-2", ""),
        };
        foreach (var (handle, data) in ready)
        {
            Handlers[handle](handle, data);
        }
    }
}
```

```output
accept conn-1
accept conn-2
conn-1: hello
conn-2: hi
close conn-1
close conn-2
```

- Verdict: idiom. The handler table maps handles to `Action<string, string>` delegates, and a tuple array gives the ready events.
