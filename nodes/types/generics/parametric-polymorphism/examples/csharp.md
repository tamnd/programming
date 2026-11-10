```csharp run id=max
T Max<T>(IEnumerable<T> items) where T : IComparable<T>
{
    T result = items.First();
    foreach (T item in items)
    {
        if (item.CompareTo(result) > 0)
        {
            result = item;
        }
    }
    return result;
}

Console.WriteLine(Max(new[] { 3, 9, 4 }));
Console.WriteLine(Max(new[] { "pear", "apple", "plum" }));

IEnumerable<object> items = new List<string> { "a", "b" };
Console.WriteLine(items.Count());
```

```output for=max
9
plum
2
```

- Verdict: native. Classes, structs, interfaces, delegates and methods take type parameters.
- The `where` clause states a constraint. Without it, the body may use only the members of `System.Object`.
- Type arguments exist at run time. `typeof(List<int>)` and `typeof(List<string>)` are different types, and reflection can read the type argument.
- `IEnumerable<out T>` is covariant, so a `List<string>` converts to `IEnumerable<object>`. A `List<T>` is invariant.
