```csharp run id=array-binarysearch
int[] xs = [12, 31, 47, 58, 90];
Console.WriteLine($"search 58: {Array.BinarySearch(xs, 58)}");
int r = Array.BinarySearch(xs, 50);
Console.WriteLine($"search 50: {r}, insert at {~r}");

var names = new List<string> { "Grace", "Ada", "Alan" };
names.Sort(StringComparer.Ordinal); // not stable
Console.WriteLine($"{string.Join(", ", names)}; Alan at {names.BinarySearch("Alan", StringComparer.Ordinal)}");

var people = new[] { ("Grace", 1906), ("Ada", 1815), ("Alan", 1912), ("Edsger", 1930) };
var byYear = people.OrderBy(p => p.Item2).Select(p => p.Item1); // LINQ OrderBy is stable
Console.WriteLine(string.Join(" < ", byYear));
```

```output for=array-binarysearch
search 58: 3
search 50: -4, insert at 3
Ada, Alan, Grace; Alan at 1
Ada < Grace < Alan < Edsger
```

- Verdict: stdlib. `Array.Sort` and `List<T>.Sort` use an introspective sort, which is not stable. LINQ's `OrderBy` is stable. `Array.BinarySearch` returns the index, or the bitwise complement of the insertion point, so `~r` gives the place to insert. The comparer must be the same for the sort and the search: here both use ordinal string order.
