```csharp run id=jobs
using System;

static int RunJob(int job, int[] items)
{
    try
    {
        return items[job] * 10;
    }
    finally
    {
        Console.WriteLine($"cleanup job {job}");
    }
}

int[] items = [1, 2];
foreach (int job in new[] { 0, 5, 1 })
{
    try
    {
        Console.WriteLine($"job {job} -> {RunJob(job, items)}");
    }
    catch (IndexOutOfRangeException e)
    {
        Console.WriteLine($"job {job} failed: {e.GetType().Name}");
    }
}
```

```output
cleanup job 0
job 0 -> 10
cleanup job 5
job 5 failed: IndexOutOfRangeException
cleanup job 1
job 1 -> 20
```

- Verdict: idiom. C# has no panic. A bug such as an index out of range throws an ordinary exception, and a boundary catches it.
- `Environment.FailFast` is the hard stop. It ends the process at once and writes the message and a stack trace.

`Environment.FailFast` ends the process before the `finally` block can run.

```csharp fails expect=runtime id=failfast
using System;

try
{
    Environment.FailFast("broken invariant");
}
finally
{
    Console.WriteLine("cleanup");
}
```
