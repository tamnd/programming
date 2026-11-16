```csharp run id=stack
var stack = new Stack<string>();
foreach (var item in new[] { "a", "b", "c" })
{
    stack.Push(item);
}
Console.WriteLine($"{string.Join(",", stack)} top {stack.Peek()}");

Console.WriteLine($"{stack.Pop()} {stack.Pop()} left {stack.Count}");
stack.Pop();
Console.WriteLine($"TryPop on empty: {(stack.TryPop(out var top) ? top : "no")}");
try
{
    stack.Pop();
}
catch (InvalidOperationException)
{
    Console.WriteLine("Pop on empty: InvalidOperationException");
}
```

```output for=stack
c,b,a top c
c b left 1
TryPop on empty: no
Pop on empty: InvalidOperationException
```

- Verdict: stdlib. `Stack<T>` has `Push`, `Pop` and `Peek`, and it enumerates from the top.
- `Pop` on an empty stack throws `InvalidOperationException`. `TryPop` returns `false` instead.
