```csharp run id=survive-crash
using System.Text;

var dir = Directory.CreateTempSubdirectory("gd").FullName;
var path = Path.Combine(dir, "orders.log");

var memory = new Dictionary<string, string>();
var durable = new DurableQueue(path);
for (int i = 1; i <= 3; i++)
{
    memory[$"m{i}"] = $"order-{i}";
    durable.Send($"m{i}", $"order-{i}");
}
Console.WriteLine($"before crash: memory {memory.Count}, durable {durable.Pending.Count}");

memory = new Dictionary<string, string>(); // a crash loses what was only in memory
durable = new DurableQueue(path);
Console.WriteLine($"after crash: memory {memory.Count}, durable {durable.Pending.Count}");

var id = durable.Ids()[0];
Console.WriteLine($"receive {id} {durable.Pending[id]}, ack");
durable.Ack(id);

durable = new DurableQueue(path);
Console.WriteLine($"after second crash: durable {durable.Pending.Count} {string.Join(" ", durable.Ids())}");
Directory.Delete(dir, true);

// A queue whose state is a log on disk: one line for each send and each ack.
sealed class DurableQueue
{
    private readonly string path;
    private readonly List<string> order = new();
    public Dictionary<string, string> Pending { get; } = new();

    public DurableQueue(string path)
    {
        this.path = path;
        if (!File.Exists(path))
        {
            return;
        }
        foreach (var line in File.ReadLines(path))
        {
            var p = line.Split(' ', 3);
            if (p[0] == "send")
            {
                Pending[p[1]] = p[2];
                order.Add(p[1]);
            }
            else
            {
                Pending.Remove(p[1]);
            }
        }
    }

    private void Log(string line)
    {
        using var f = new FileStream(path, FileMode.Append, FileAccess.Write);
        var bytes = Encoding.UTF8.GetBytes(line + "\n");
        f.Write(bytes);
        f.Flush(flushToDisk: true);
    }

    public void Send(string id, string body)
    {
        Log($"send {id} {body}");
        Pending[id] = body;
        order.Add(id);
    }

    public void Ack(string id)
    {
        Log($"ack {id}");
        Pending.Remove(id);
    }

    // The pending ids in the order they were sent.
    public List<string> Ids() => order.Where(Pending.ContainsKey).ToList();
}
```

```output
before crash: memory 3, durable 3
after crash: memory 0, durable 3
receive m1 order-1, ack
after second crash: durable 2 m2 m3
```

- Verdict: idiom. .NET has no persistent queue. A log file with `FileStream.Flush(flushToDisk: true)` after each line is the store.
- `Flush()` with no argument only empties the buffer of the stream into the operating system. The argument `true` also asks the operating system to write the data to the disk.
- `Directory.CreateTempSubdirectory` gives the log a new directory for each run, and the program deletes it at the end.
