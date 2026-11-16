```csharp run id=deque
var d = new LinkedList<string>(new[] { "b", "c" });
d.AddFirst("a");
d.AddLast("d");
Console.WriteLine($"{string.Join(",", d)} front {d.First!.Value} back {d.Last!.Value}");

var front = d.First!.Value;
d.RemoveFirst();
var back = d.Last!.Value;
d.RemoveLast();
Console.WriteLine($"{front} {back} {string.Join(",", d)}");

try
{
    d.Clear();
    d.RemoveFirst();
}
catch (InvalidOperationException)
{
    Console.WriteLine("RemoveFirst on empty: InvalidOperationException");
}
```

```output for=deque
a,b,c,d front a back d
a d b,c
RemoveFirst on empty: InvalidOperationException
```

- Verdict: stdlib. .NET has no array-based deque type. `LinkedList<T>` adds and removes at both ends with `AddFirst`, `AddLast`, `RemoveFirst` and `RemoveLast`.
- `First` and `Last` are `null` on an empty list, and `RemoveFirst` throws `InvalidOperationException`.
