```csharp run id=local-functions
Report("The cat and the hat and the bat", 2);

static string[] SplitWords(string text) =>
    text.ToLowerInvariant().Split(' ', StringSplitOptions.RemoveEmptyEntries);

static Dictionary<string, int> CountWords(string[] words)
{
    var counts = new Dictionary<string, int>();
    foreach (var w in words)
    {
        counts[w] = counts.GetValueOrDefault(w) + 1;
    }
    return counts;
}

static IEnumerable<KeyValuePair<string, int>> Top(Dictionary<string, int> counts, int n) =>
    counts.OrderByDescending(kv => kv.Value).ThenBy(kv => kv.Key).Take(n);

static void Report(string text, int n)
{
    foreach (var (word, c) in Top(CountWords(SplitWords(text)), n))
    {
        Console.WriteLine($"{word,-5} {c}");
    }
}
```

```output for=local-functions
the   3
and   2
```

- Verdict: idiom. Methods belong to classes, but a file with top-level statements can declare local functions, as here, and a `static class` holds procedures that do not need an object.
