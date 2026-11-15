```csharp run id=tuples-iterator-method-and-changed-list
var items = new List<(string Name, int Qty)> { ("tea", 2), ("cake", 1) };
foreach (var (name, qty) in items)
    Console.WriteLine($"{name} x{qty}");

// foreach works on any IEnumerable<T>, here an iterator method.
Console.WriteLine(string.Join(" ", Evens(8)));

var list = new List<int> { 1, 2, 3 };
try
{
    foreach (var x in list)
        if (x == 2) list.Add(4);
}
catch (InvalidOperationException)
{
    Console.WriteLine("InvalidOperationException: the list changed");
}

static IEnumerable<int> Evens(int limit)
{
    for (int i = 0; i <= limit; i += 2)
        yield return i;
}
```

```output for=tuples-iterator-method-and-changed-list
tea x2
cake x1
0 2 4 6 8
InvalidOperationException: the list changed
```

- Verdict: native. `foreach (var (name, qty) in items)` takes each tuple apart. `Evens` is an iterator method, and `yield return` gives the elements one by one to the caller. An `Add` during the loop makes the enumerator of the list throw an `InvalidOperationException` at its next step.
