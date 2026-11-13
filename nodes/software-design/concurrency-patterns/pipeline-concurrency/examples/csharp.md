```csharp run id=pipeline-concurrency
using System;
using System.Linq;
using System.Threading;
using System.Threading.Channels;
using System.Threading.Tasks;

class Program
{
    const int Items = 20;
    const int Workers = 3;

    static async Task Main()
    {
        // Bounded channels: a writer waits while the buffer is full.
        var numbers = Channel.CreateBounded<int>(1); // source -> square workers
        var squares = Channel.CreateBounded<int>(1); // square workers -> sink
        int sinkStarted = 0;
        bool overlap = false;

        var source = Task.Run(async () =>
        {
            for (int n = 1; n <= Items; n++) await numbers.Writer.WriteAsync(n);
            // Each stage can hold only a few values, so the sink must already be running.
            overlap = Volatile.Read(ref sinkStarted) == 1;
            numbers.Writer.Complete();
        });
        var workers = Enumerable.Range(0, Workers).Select(_ => Task.Run(async () => // fan-out
        {
            await foreach (int n in numbers.Reader.ReadAllAsync()) await squares.Writer.WriteAsync(n * n);
        })).ToArray();
        // The channel closes when all workers are done: fan-in.
        var closer = Task.WhenAll(workers).ContinueWith(_ => squares.Writer.Complete());

        int received = 0, total = 0;
        await foreach (int value in squares.Reader.ReadAllAsync())
        {
            Volatile.Write(ref sinkStarted, 1);
            received++;
            total += value;
        }
        await Task.WhenAll(source, closer);

        Console.WriteLine("items from the source: " + Items);
        Console.WriteLine("square workers: " + Workers);
        Console.WriteLine("squares that reached the sink: " + received);
        Console.WriteLine("sum of squares: " + total);
        Console.WriteLine("the sink got a value before the source sent its last: " + (overlap ? "True" : "False"));
    }
}
```

```output
items from the source: 20
square workers: 3
squares that reached the sink: 20
sum of squares: 2870
the sink got a value before the source sent its last: True
```

- Verdict: stdlib. `System.Threading.Channels` with `Channel.CreateBounded`, `ReadAllAsync` and `Writer.Complete()` to close a stage.
