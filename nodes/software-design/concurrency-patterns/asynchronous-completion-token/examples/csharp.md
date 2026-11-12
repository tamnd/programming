```csharp run id=act
using System;
using System.Collections.Generic;
using System.Linq;

static class Program
{
    static IEnumerable<(int Token, string Result)> Service(List<(int Token, string Name)> requests) =>
        requests.AsEnumerable().Reverse().Select(r => (r.Token, r.Name + " loaded"));

    static void Main()
    {
        var pending = new Dictionary<int, string>();
        var requests = new List<(int Token, string Name)>();
        string[] names = { "image-7", "image-9" };
        for (int i = 0; i < names.Length; i++)
        {
            int token = i + 1;
            pending[token] = names[i];
            requests.Add((token, names[i]));
            Console.WriteLine($"send {token}: load {names[i]}");
        }
        foreach (var (token, result) in Service(requests))
        {
            pending.Remove(token); // the token finds the state at once
            Console.WriteLine($"done {token}: {result}");
        }
    }
}
```

```output
send 1: load image-7
send 2: load image-9
done 2: image-9 loaded
done 1: image-7 loaded
```

- Verdict: idiom. Named tuples carry the token, and a `Dictionary<int, string>` holds the pending state.
