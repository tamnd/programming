```csharp run id=either
foreach (object v in new object[] { 21, "ok" })
{
    Console.WriteLine(Describe(v));
}

static string Describe(object value) => value switch
{
    int n => $"number {n}, doubled {n * 2}",
    string s => $"text {s}, upper {s.ToUpperInvariant()}",
    _ => throw new ArgumentException("not a number or a string"),
};
```

```output for=either
number 21, doubled 42
text ok, upper OK
```

- Verdict: native. C# 15 has `union` types, such as `union Pet(Cat, Dog, Bird)`. This example uses `object` and type patterns, which work in older versions too.
- A `switch` expression with type patterns tests the type and binds the value.
- With `object`, the compiler cannot know the set of types, so the `switch` needs a `_` branch.
