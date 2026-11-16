```csharp run id=pipeline
static int Square(int n)
{
    Console.WriteLine($"square {n}");
    return n * n;
}

var oddSquares = Enumerable.Range(1, int.MaxValue)
    .Select(Square)
    .Where(s => s % 2 == 1)
    .Take(2);
Console.WriteLine("query built");
Console.WriteLine(string.Join(", ", oddSquares));
Console.WriteLine(oddSquares.Sum());
```

```output for=pipeline
query built
square 1
square 2
square 3
1, 9
square 1
square 2
square 3
10
```

- Verdict: stdlib. LINQ operators such as `Select`, `Where` and `Take` use deferred execution: building the query does no work, and each enumeration runs it.
- The query is a recipe, not a result. `Sum` enumerates it a second time, so `Square` runs again. Call `ToList` once when a result is needed more than one time.
