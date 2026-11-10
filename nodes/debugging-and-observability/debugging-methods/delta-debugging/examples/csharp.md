```csharp run id=ddmin
int tests = 0;

// Stands for one run of the browser: it crashes on a <SELECT ...> tag.
bool BrowserCrashes(string text)
{
    tests++;
    int start = text.IndexOf("<SELECT", StringComparison.Ordinal);
    return start >= 0 && text.IndexOf('>', start) >= 0;
}

string page = "<td align=left><SELECT NAME=\"priority\" MULTIPLE SIZE=7>";
string minimal = DeltaDebugging.Ddmin(page, BrowserCrashes);
Console.WriteLine($"{page.Length} characters reduced to {minimal} in {tests} tests");

static class DeltaDebugging
{
    // Returns a 1-minimal part of chars on which fails is still true.
    public static string Ddmin(string chars, Func<string, bool> fails)
    {
        int n = 2;
        while (chars.Length >= 2)
        {
            int size = chars.Length;
            var bounds = Enumerable.Range(0, n).Select(i => (Lo: i * size / n, Hi: (i + 1) * size / n)).ToList();
            var subsets = bounds.Select(b => chars[b.Lo..b.Hi]);
            var complements = bounds.Select(b => chars[..b.Lo] + chars[b.Hi..]);
            string? smaller = subsets.FirstOrDefault(fails);
            if (smaller != null) // reduce to subset
            {
                chars = smaller;
                n = 2;
                Console.WriteLine($"subset     {chars.Length}");
                continue;
            }
            smaller = complements.FirstOrDefault(fails);
            if (smaller != null) // reduce to complement
            {
                chars = smaller;
                n = Math.Max(n - 1, 2);
                Console.WriteLine($"complement {chars.Length}");
                continue;
            }
            if (n == size) // each single character was removed
            {
                break;
            }
            n = Math.Min(size, 2 * n); // increase granularity
        }
        return chars;
    }
}
```

```output for=ddmin
complement 42
complement 28
complement 21
complement 17
complement 14
complement 13
complement 12
complement 10
complement 9
complement 8
55 characters reduced to <SELECT> in 147 tests
```

- Verdict: idiom. .NET has no delta debugging.
- Ranges such as `chars[lo..hi]` and `chars[..lo]` cut the string.
- `Select` is lazy, so `FirstOrDefault(fails)` builds and tests parts only until one fails. It returns `null` when no part fails.
- A local function in the top-level statements counts the tests.
