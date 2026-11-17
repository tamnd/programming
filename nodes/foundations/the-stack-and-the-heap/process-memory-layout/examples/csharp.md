```csharp run id=regions
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;

static class Demo
{
    static void Main()
    {
        var found = new Dictionary<string, ulong>();
        foreach (var line in File.ReadLines("/proc/self/maps"))
        {
            var fields = line.Split(' ', StringSplitOptions.RemoveEmptyEntries);
            var path = fields.Length > 5 ? fields[5] : "";
            if ((path == "[heap]" || path == "[stack]" || path == "[vdso]") && !found.ContainsKey(path))
            {
                found[path] = ulong.Parse(fields[0].Split('-')[0], NumberStyles.HexNumber);
                Console.WriteLine($"{path} {fields[1]}");
            }
        }
        var below = found["[heap]"] < found["[stack]"];
        Console.WriteLine($"the heap is below the stack: {below.ToString().ToLowerInvariant()}");
    }
}
```

```output for=regions
[heap] rw-p
[stack] rw-p
[vdso] r-xp
the heap is below the stack: true
```

- Verdict: native. The .NET runtime runs in a process with the usual layout. Safe C# code has no addresses, and the program reads `/proc/self/maps` as a file.
