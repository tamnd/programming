```csharp run id=job-loop-and-checked-wrap
var queue = new Queue<string>(new[] { "job1", "job2", "stop", "job3" });
while (true)
{
    var job = queue.Dequeue();
    if (job == "stop") break;
    Console.WriteLine($"run {job}");
}
Console.WriteLine($"left in queue: {queue.Count}");

// checked turns a silent wrap-around into an exception.
try
{
    checked
    {
        for (byte b = 250; b != 0; b++) { }
    }
}
catch (OverflowException)
{
    Console.WriteLine("overflow instead of a wrap to 0");
}

// Without checked, the byte wraps to 0 and the loop ends.
int passes = 0;
for (byte b = 250; b != 0; b++) passes++;
Console.WriteLine($"unchecked passes: {passes}");
```

```output for=job-loop-and-checked-wrap
run job1
run job2
left in queue: 1
overflow instead of a wrap to 0
unchecked passes: 6
```

- Verdict: idiom. The job loop is `while (true)` with a `break` at the stop job, so `job3` stays in the queue. The loop `for (byte b = 250; b != 0; b++)` only looks endless: by default the byte wraps from 255 to 0 after 6 passes. In a `checked` block, the step from 255 throws an `OverflowException`.
