```csharp run id=master-worker-pattern
using System;
using System.Linq;
using System.Threading.Tasks;

class Program
{
    const int Limit = 20000;
    const int Workers = 4;

    // The work of one worker: count the primes in [lo, hi).
    static int CountPrimes(int lo, int hi)
    {
        int count = 0;
        for (int n = Math.Max(lo, 2); n < hi; n++)
        {
            int d = 2;
            while (d * d <= n && n % d != 0) d++;
            if (d * d > n) count++;
        }
        return count;
    }

    static void Main()
    {
        // 1. Split the work into one part for each worker.
        var parts = Enumerable.Range(0, Workers)
            .Select(i => (Lo: Limit * i / Workers, Hi: Limit * (i + 1) / Workers))
            .ToArray();
        // 2. Start a task for each part, then wait for all of them.
        var tasks = parts.Select(p => Task.Run(() => CountPrimes(p.Lo, p.Hi))).ToArray();
        int[] results = Task.WhenAll(tasks).Result;
        // 3. Combine the partial results.
        int total = results.Sum();

        Console.WriteLine("parts: [" + string.Join(", ", parts.Select(p => $"({p.Lo}, {p.Hi})")) + "]");
        Console.WriteLine("primes in each part: [" + string.Join(", ", results) + "]");
        Console.WriteLine($"primes below {Limit} from the master: {total}");
        Console.WriteLine("same as one loop: " + (total == CountPrimes(0, Limit) ? "True" : "False"));
    }
}
```

```output
parts: [(0, 5000), (5000, 10000), (10000, 15000), (15000, 20000)]
primes in each part: [669, 560, 525, 508]
primes below 20000 from the master: 2262
same as one loop: True
```

- Verdict: stdlib. `Task.Run` for each part and `Task.WhenAll` to collect the results in order.
