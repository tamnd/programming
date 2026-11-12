```csharp run id=thread-confinement
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;

class Program
{
    static void Main()
    {
        string[] words = { "a", "b", "c", "d", "e" };
        int chunks = 4;
        var results = new Dictionary<string, int>[chunks];
        var threads = new Thread[chunks];
        for (int i = 0; i < chunks; i++)
        {
            int index = i;
            threads[i] = new Thread(() =>
            {
                var counts = new Dictionary<string, int>(); // local: no lock
                for (int r = 0; r < 2000; r++)
                    foreach (var w in words)
                        counts[w] = counts.GetValueOrDefault(w) + 1;
                results[index] = counts; // hand the finished object over
            });
            threads[i].Start();
        }
        foreach (var t in threads) t.Join(); // after Join, Main owns the results

        var total = new Dictionary<string, int>();
        foreach (var counts in results)
            foreach (var (w, n) in counts)
                total[w] = total.GetValueOrDefault(w) + n;
        Console.WriteLine("chunks: " + chunks);
        Console.WriteLine("words counted: " + total.Values.Sum());
        Console.WriteLine("distinct words: " + total.Count);
        Console.WriteLine("count of 'a': " + total["a"]);
    }
}
```

```output
chunks: 4
words counted: 40000
distinct words: 5
count of 'a': 8000
```

- Verdict: idiom. Each thread fills a local `Dictionary`, and `Join` makes the main thread the only owner of the results.
