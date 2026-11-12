```csharp run id=callbacks
using System;
using System.Collections.Generic;

static void ForEach<T>(IEnumerable<T> items, Action<T> callback)
{
    foreach (var item in items) callback(item); // a blocking callback
}

ForEach(new[] { "hello", "world" }, word => Console.WriteLine(word.ToUpper()));

var pending = new Queue<Action>();
pending.Enqueue(() => Console.WriteLine("saved")); // a deferred callback
Console.WriteLine("request sent");
while (pending.Count > 0) pending.Dequeue()();
```

```output for=callbacks
HELLO
WORLD
request sent
saved
```

- Verdict: native. Delegates such as `Action<T>` are the callback types.
