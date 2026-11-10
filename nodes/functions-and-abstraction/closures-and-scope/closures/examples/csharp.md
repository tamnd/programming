```csharp run id=counter
Func<int> MakeCounter()
{
    int count = 0;
    return () => ++count;
}

var first = MakeCounter();
var second = MakeCounter();
Console.WriteLine($"{first()} {first()} {first()}");
Console.WriteLine(second());
```

```output for=counter
1 2 3
1
```

- Verdict: native. Lambda expressions, anonymous methods and local functions capture outer variables.
- The closure captures the variable, so the lambda can assign to it. The variable lives at least as long as the delegate that refers to it.
- A `for` loop has one variable for all iterations, so closures made in the loop see its last value. Each iteration of `foreach` has its own variable.

```csharp wrong id=for-loop
var callbacks = new List<Func<int>>();
for (int i = 0; i < 3; i++)
{
    callbacks.Add(() => i);
}
Console.WriteLine(string.Join(" ", callbacks.Select(f => f())));
```

```output for=for-loop
3 3 3
```

```csharp run id=foreach-loop
var callbacks = new List<Func<int>>();
foreach (int i in Enumerable.Range(0, 3))
{
    callbacks.Add(() => i);
}
Console.WriteLine(string.Join(" ", callbacks.Select(f => f())));
```

```output for=foreach-loop
0 1 2
```
