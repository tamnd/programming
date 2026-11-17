```csharp run id=stack-adt
using System;
using System.Collections.Generic;

static string Drain(IStack<int> s)
{
    for (int v = 1; v <= 3; v++) s.Push(v);
    var output = new List<int>();
    while (!s.IsEmpty) output.Add(s.Pop());
    return string.Join(" ", output);
}

IStack<int>[] stacks = { new ArrayStack<int>(), new LinkedStack<int>() };
foreach (var s in stacks)
{
    Console.WriteLine($"{s.GetType().Name.Split('`')[0]}: {Drain(s)}");
}

interface IStack<T>
{
    void Push(T v);
    T Pop();
    bool IsEmpty { get; }
}

sealed class ArrayStack<T> : IStack<T>
{
    private readonly List<T> items = new();
    public void Push(T v) => items.Add(v);
    public T Pop() { var v = items[items.Count - 1]; items.RemoveAt(items.Count - 1); return v; }
    public bool IsEmpty => items.Count == 0;
}

sealed class LinkedStack<T> : IStack<T>
{
    private sealed record Cell(T V, Cell? Rest);
    private Cell? top;
    public void Push(T v) => top = new Cell(v, top);
    public T Pop() { var c = top!; top = c.Rest; return c.V; }
    public bool IsEmpty => top is null;
}
```

```output for=stack-adt
ArrayStack: 3 2 1
LinkedStack: 3 2 1
```

- Verdict: native. The interface `IStack<T>` names the operations, and each class keeps its representation in `private` members.
- `GetType().Name` of a generic class ends with a backtick and the number of type parameters, so the code cuts the name there.
