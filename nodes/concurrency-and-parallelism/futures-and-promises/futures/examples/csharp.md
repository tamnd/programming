```csharp run id=tasks
var price = Task.Run(() => 40);
var tax = Task.Run(() => 2);
int[] parts = await Task.WhenAll(price, tax);
Console.WriteLine(parts.Sum());

var source = new TaskCompletionSource<string>();
_ = Task.Run(() => source.SetResult("done"));
Console.WriteLine(await source.Task);

try
{
    await Task.Run(() => throw new InvalidOperationException("no stock"));
}
catch (InvalidOperationException e)
{
    Console.WriteLine("caught: " + e.Message);
}
```

```output for=tasks
42
done
caught: no stock
```

- Verdict: native. `Task<T>` is the future type of .NET, and `await` waits for it without a blocked thread. `TaskCompletionSource<T>` is the promise side.
- A task that a method returns is already started. `await` rethrows the exception of the task.
- `.Result` and `.Wait()` block the thread. In code with a synchronization context, such as a UI thread, they can deadlock.
