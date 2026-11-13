```csharp run id=barrier-pattern
using System;
using System.Linq;
using System.Threading;

class Program
{
    const int N = 8;
    static readonly int[] values = Enumerable.Range(1, N).ToArray();
    static readonly int[] distances = { 1, 2, 4 }; // each step adds the value that is d places to the left

    static void Main()
    {
        Console.WriteLine("input: [" + string.Join(", ", values) + "]");
        int trips = 0;
        // The action runs once per phase, before the threads are released.
        using var barrier = new Barrier(N, b => trips++);

        var threads = Enumerable.Range(0, N).Select(i => new Thread(() =>
        {
            foreach (int d in distances)
            {
                // Phase 1: read a value that the last step wrote.
                int add = i >= d ? values[i - d] : 0;
                barrier.SignalAndWait(); // nobody writes until every worker has read
                // Phase 2: write the new value.
                values[i] += add;
                barrier.SignalAndWait(); // nobody reads until every worker has written
            }
        })).ToList();
        foreach (var t in threads) t.Start();
        foreach (var t in threads) t.Join();

        bool matches = Enumerable.Range(0, N).All(i => values[i] == (i + 1) * (i + 2) / 2);
        Console.WriteLine("steps: " + distances.Length);
        Console.WriteLine("barrier trips: " + trips);
        Console.WriteLine("prefix sums: [" + string.Join(", ", values) + "]");
        Console.WriteLine("matches a running total: " + (matches ? "True" : "False"));
    }
}
```

```output
input: [1, 2, 3, 4, 5, 6, 7, 8]
steps: 3
barrier trips: 6
prefix sums: [1, 3, 6, 10, 15, 21, 28, 36]
matches a running total: True
```

- Verdict: stdlib. `System.Threading.Barrier` with a post-phase action and `SignalAndWait`.
