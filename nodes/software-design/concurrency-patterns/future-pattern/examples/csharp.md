```csharp run id=future-pattern
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

class Program
{
    static void Main()
    {
        string[] items = { "apple", "bread", "cheese" };
        var prices = new Dictionary<string, int> { ["apple"] = 12, ["bread"] = 7, ["cheese"] = 30 };
        // The lookups wait here with the caller, so the output does not depend on timing.
        var meeting = new Barrier(4);

        // Each call returns a task at once. The lookup runs on its own thread.
        var tasks = items.Select(item => Task.Factory.StartNew(() =>
        {
            meeting.SignalAndWait(TimeSpan.FromSeconds(5)); // returns when all 3 lookups and the caller are here
            return prices[item];
        }, TaskCreationOptions.LongRunning)).ToList();
        Console.WriteLine("lookups started: " + tasks.Count);
        Console.WriteLine("done right after the calls: " + (tasks.Any(t => t.IsCompleted) ? "True" : "False"));
        bool met = meeting.SignalAndWait(TimeSpan.FromSeconds(5)); // the caller is free to do other work here
        Console.WriteLine("all three lookups ran at once: " + (met ? "True" : "False"));
        var results = tasks.Select(t => t.Result).ToList(); // wait for each result

        Console.WriteLine("prices: [" + string.Join(", ", results) + "]");
        Console.WriteLine("total: " + results.Sum());
    }
}
```

```output
lookups started: 3
done right after the calls: False
all three lookups ran at once: True
prices: [12, 7, 30]
total: 49
```

- Verdict: stdlib. `Task.Factory.StartNew` returns a `Task<int>` at once; `IsCompleted` checks it and `Result` waits for the value.
