```csharp run id=competing-workers
using System.Collections.Concurrent;

const int MaxDeliveries = 3, Workers = 3;
var jobs = new BlockingCollection<(int Job, int Attempt)>();
var gate = new object();
var results = new SortedDictionary<int, int>();
var events = new List<(int Job, int Attempt, string Text)>();
int pending = 8, deliveries = 0;

int Handle(int job, int attempt)
{
    Thread.Sleep(40);
    if (job == 4 && attempt == 1) throw new TimeoutException("timeout of the database");
    if (job == 7) throw new FormatException("bad payload");
    return job * job;
}

void Worker()
{
    foreach (var (job, attempt) in jobs.GetConsumingEnumerable())
    {
        lock (gate) deliveries++;
        int? result = null;
        Exception? error = null;
        try { result = Handle(job, attempt); } catch (Exception e) { error = e; }
        lock (gate)
        {
            if (error != null && attempt < MaxDeliveries)
            {
                events.Add((job, attempt, $"failed on delivery {attempt} ({error.Message}), back on the queue"));
                jobs.Add((job, attempt + 1));
                continue;
            }
            if (error != null)
                events.Add((job, attempt, $"failed on delivery {attempt} ({error.Message}), to the dead letter channel"));
            else
                results[job] = result!.Value;
            if (--pending == 0) jobs.CompleteAdding();
        }
    }
}

for (int job = 1; job <= 8; job++) jobs.Add((job, 1));
var threads = Enumerable.Range(0, Workers).Select(_ => new Thread(Worker)).ToList();
threads.ForEach(t => t.Start());
threads.ForEach(t => t.Join());

Console.WriteLine($"{Workers} workers, 8 jobs on one queue");
foreach (var e in events.OrderBy(e => e.Job).ThenBy(e => e.Attempt))
    Console.WriteLine($"job {e.Job}: {e.Text}");
Console.WriteLine($"done: {results.Count} jobs, results {string.Join(" ", results.Values)}");
Console.WriteLine($"deliveries: {deliveries}");
```

```output
3 workers, 8 jobs on one queue
job 4: failed on delivery 1 (timeout of the database), back on the queue
job 7: failed on delivery 1 (bad payload), back on the queue
job 7: failed on delivery 2 (bad payload), back on the queue
job 7: failed on delivery 3 (bad payload), to the dead letter channel
done: 7 jobs, results 1 4 9 16 25 36 64
deliveries: 11
```

- Verdict: idiom. `BlockingCollection<T>` with `GetConsumingEnumerable` gives each item to one of the threads that read it.
- `CompleteAdding` after the last finished job ends the loop in each thread.
- A `lock` on one object protects the counters, the results and the events.
