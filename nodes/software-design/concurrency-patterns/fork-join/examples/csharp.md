```csharp run id=fork-join
using System;
using System.Threading;
using System.Threading.Tasks;

class Program
{
    const long Threshold = 10_000; // a range this small is summed directly
    static int leaves;
    static long largest;
    static readonly object gate = new object();

    // Returns the sum of lo, lo + 1, ..., hi - 1.
    static long Solve(long lo, long hi)
    {
        if (hi - lo <= Threshold)
        {
            Interlocked.Increment(ref leaves);
            lock (gate) largest = Math.Max(largest, hi - lo);
            long s = 0;
            for (long i = lo; i < hi; i++) s += i;
            return s;
        }
        long mid = (lo + hi) / 2;
        long left = 0, right = 0;
        // Fork both halves and join them when Parallel.Invoke returns.
        Parallel.Invoke(() => left = Solve(lo, mid), () => right = Solve(mid, hi));
        return left + right;
    }

    static void Main()
    {
        long n = 1_000_000;
        long total = Solve(1, n + 1);
        Console.WriteLine("sum: " + total);
        Console.WriteLine("leaf tasks: " + leaves);
        Console.WriteLine("largest leaf, at most 10000: " + (largest <= Threshold ? "True" : "False"));
        Console.WriteLine("matches n(n+1)/2: " + (total == n * (n + 1) / 2 ? "True" : "False"));
    }
}
```

```output
sum: 500000500000
leaf tasks: 128
largest leaf, at most 10000: True
matches n(n+1)/2: True
```

- Verdict: stdlib. `Parallel.Invoke` runs the two halves and returns when both are done.
