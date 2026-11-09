```csharp run id=load
async Task<string> Load(string name, int ms)
{
    Console.WriteLine($"start {name}");
    await Task.Delay(ms);
    Console.WriteLine($"end {name}");
    return name;
}

string[] results = await Task.WhenAll(Load("slow", 200), Load("fast", 100));
Console.WriteLine(string.Join(", ", results));
```

```output for=load
start slow
start fast
end fast
end slow
slow, fast
```

- Verdict: native. An `async` method returns `Task`, `Task<T>` or `ValueTask<T>`, and `await` suspends it until the awaited task completes.
- The method runs at once, up to the first `await` of a task that is not complete.
- `Task.WhenAll` waits for all the tasks. The continuations run on thread pool threads in a console program, so the code between two awaits can run on different threads.
- A top-level `await` in the main file makes the generated `Main` method async.
