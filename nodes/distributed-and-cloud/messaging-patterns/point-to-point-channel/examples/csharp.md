```csharp run id=competing-workers
using System.Collections.Concurrent;
using System.Threading.Channels;

var jobs = Channel.CreateUnbounded<string>();
var done = new ConcurrentBag<(string Job, string Worker)>();

var workers = Enumerable.Range(1, 3).Select(i => Task.Run(async () =>
{
    // Each item is read by one reader only.
    await foreach (var job in jobs.Reader.ReadAllAsync())
    {
        done.Add((job, $"w{i}"));
    }
})).ToArray();

for (var n = 1; n <= 9; n++)
{
    await jobs.Writer.WriteAsync($"job-{n}");
}
jobs.Writer.Complete();
await Task.WhenAll(workers);

var names = done.Select(d => d.Job).Order().ToList();
Console.WriteLine("handled: " + string.Join(" ", names));
Console.WriteLine($"each job once: {names.Count == 9 && names.Distinct().Count() == 9}");
```

```output
handled: job-1 job-2 job-3 job-4 job-5 job-6 job-7 job-8 job-9
each job once: True
```

- Verdict: stdlib. A `Channel<T>` from `System.Threading.Channels` gives each item to one reader, and three tasks that run `ReadAllAsync` share the jobs.
- `Writer.Complete()` ends the loops of the readers when the channel is empty.
- `ConcurrentBag` collects the results from the three tasks with no lock. The order of the bag is not defined, so the program sorts the jobs.
