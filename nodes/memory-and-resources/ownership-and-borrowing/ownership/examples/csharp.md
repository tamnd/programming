```csharp run id=span
int[] samples = [1, 2, 3, 4, 5, 6];
Span<int> middle = samples.AsSpan(2, 3);
Scale(middle, 10);
Console.WriteLine(string.Join(",", samples));
Console.WriteLine(Sum(samples));

static void Scale(Span<int> values, int factor)
{
    foreach (ref int value in values)
    {
        value *= factor;
    }
}

static int Sum(ReadOnlySpan<int> values)
{
    int total = 0;
    foreach (int value in values)
    {
        total += value;
    }
    return total;
}
```

```output for=span
1,2,30,40,50,6
129
```

- Verdict: absent. The garbage collector owns all objects. `Span<T>` and `ReadOnlySpan<T>` are borrows of a part of an array or of other memory, and they do not copy.
- A span is a `ref struct`. The compiler does not let it escape to the heap, for example as a field of a class, so a span cannot outlive the method that gets it.
- For other resources, the convention is that the code which makes an `IDisposable` disposes of it, usually with `using`.
