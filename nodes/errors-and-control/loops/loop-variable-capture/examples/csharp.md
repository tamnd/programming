```csharp run id=foreach-against-for-and-a-copy
static string Run(List<Func<int>> fs) => string.Join(" ", fs.Select(f => f()));

// foreach: each pass has its own variable (C# 5 and later).
var fromForeach = new List<Func<int>>();
foreach (var v in new[] { 100, 110, 120 })
{
    fromForeach.Add(() => v);
}
Console.WriteLine($"foreach: {Run(fromForeach)}");

// for: one variable for the whole loop.
var fromFor = new List<Func<int>>();
for (int i = 0; i < 3; i++)
{
    fromFor.Add(() => i);
}
Console.WriteLine($"for: {Run(fromFor)}");

// The fix for for: a copy declared in the body.
var fixedFor = new List<Func<int>>();
for (int i = 0; i < 3; i++)
{
    int copy = i;
    fixedFor.Add(() => copy);
}
Console.WriteLine($"for with a copy: {Run(fixedFor)}");
```

```output for=foreach-against-for-and-a-copy
foreach: 100 110 120
for: 3 3 3
for with a copy: 0 1 2
```

- Verdict: native. This is the example from Eric Lippert's post: with C# 5 and later, `foreach` declares `v` inside the loop, so the three lambdas return 100, 110 and 120. The `for` loop did not change, so its lambdas share `i` and return 3, the value that ended the loop. A variable declared in the body is new in each pass.
