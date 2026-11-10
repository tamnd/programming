```csharp run id=shared-connection
var conn = new Connection();
var workers = new List<Task>();
for (int i = 0; i < 4; i++)
{
    Connection mine = conn.Retain();
    workers.Add(Task.Run(mine.Release));
}
await Task.WhenAll(workers);
Console.WriteLine($"open after workers: {conn.IsOpen}");
conn.Release();
Console.WriteLine($"open after last release: {conn.IsOpen}");

class Connection
{
    private int users = 1;

    public bool IsOpen => Volatile.Read(ref users) > 0;

    public Connection Retain()
    {
        Interlocked.Increment(ref users);
        return this;
    }

    public void Release()
    {
        if (Interlocked.Decrement(ref users) == 0)
        {
            Console.WriteLine("connection closed");
        }
    }
}
```

```output
open after workers: True
connection closed
open after last release: False
```

- Verdict: idiom. .NET frees memory with a tracing collector, so C# objects have no reference counts. A count still helps when several users share one resource.
- `Interlocked.Decrement` decrements the field and returns the result as one atomic operation, so only one caller sees zero.
- Each task gets its reference before it starts. Without that, a fast task could close the connection while the loop still hands it out.
- For one owner, the `using` statement disposes the object at the end of its scope.
