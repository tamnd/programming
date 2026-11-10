```csharp run id=countdown
using System;
using System.Collections.Generic;

static IEnumerable<int> Countdown(int start)
{
    for (int i = start; i > 0; i--)
    {
        Console.WriteLine($"yield {i}");
        yield return i;
    }
}

IEnumerable<int> numbers = Countdown(2);
Console.WriteLine("nothing ran yet");
foreach (int n in numbers)
{
    Console.WriteLine(n);
}

using IEnumerator<string> e = new List<string> { "a", "b" }.GetEnumerator();
while (e.MoveNext())
{
    Console.WriteLine(e.Current);
}
```

```output for=countdown
nothing ran yet
yield 2
2
yield 1
1
a
b
```

- Verdict: native. An enumerable has `GetEnumerator()`. The enumerator starts before the first element; `MoveNext()` moves it and returns `false` at the end, and `Current` gives the element.
- A method with `yield return` is an iterator method. The call does not run the body. The body runs up to each `yield return` when `MoveNext()` is called, so `nothing ran yet` comes first.
- `foreach` calls `Dispose()` on the enumerator when the loop ends. The `using` declaration does the same for the manual loop.
- A change to a `List<T>` invalidates its enumerators. The next `MoveNext()` throws `InvalidOperationException` (see Pitfalls on the concept page).
