```csharp run id=ienumerable-yield
using System;
using System.Collections.Generic;
using System.Linq;

class Program
{
    static int produced = 0;

    // yield return makes a lazy IEnumerable<T>. The body runs only while a loop asks for items.
    static IEnumerable<int> Naturals()
    {
        for (int n = 0; ; n++)
        {
            produced++;
            yield return n;
        }
    }

    static void Main()
    {
        var firstSquares = Naturals().Select(n => n * n).Take(5);
        Console.WriteLine($"produced before: {produced}");
        Console.WriteLine($"{string.Join(",", firstSquares)} produced after: {produced}");

        // A LINQ query is deferred: it reads the list each time it runs, so it sees later changes.
        var list = new List<int> { 1, 2, 3 };
        IEnumerable<int> big = list.Where(x => x > 1);
        list.Add(10);
        Console.WriteLine($"{string.Join(",", big)} {big.Count()}");

        // ToList runs the query once and keeps the result.
        List<int> snapshot = big.ToList();
        list.Add(20);
        Console.WriteLine($"{snapshot.Count} {big.Count()}");

        // Arrays, lists and strings are all IEnumerable<T>. IReadOnlyList<T> adds Count and an indexer.
        IReadOnlyList<char> chars = "abc".ToList();
        IEnumerable<int>[] seqs = { new[] { 1, 2 }, new List<int> { 3 }, Enumerable.Range(4, 2) };
        Console.WriteLine($"{chars.Count} {chars[^1]} {string.Join(",", seqs.SelectMany(s => s))}");
    }
}
```

```output
produced before: 0
0,1,4,9,16 produced after: 5
2,3,10 3
3 4
3 c 1,2,3,4,5
```

- Verdict: native. A method with yield return gives an IEnumerable<T> that runs only when a loop asks for items, so the counter is 0 until Join reads the query. A LINQ query is deferred, so it sees an item added after it was made, and ToList takes a snapshot. ToList on a string gives a List<char>, which is an IReadOnlyList<char>. SelectMany joins an array, a list and a range into one sequence.
