```csharp run id=extension-block-and-property
using System;
using System.Collections.Generic;
using System.Linq;

string s = "Hello extension members";
Console.WriteLine($"{s.WordCount()} {s.IsBlank} {"  ".IsBlank}");
int[] xs = [3, 1, 2];
Console.WriteLine($"{xs.SecondLargest()} {TextExtensions.WordCount("one two")}");

static class TextExtensions
{
    extension(string text) // an extension block, new in C# 14
    {
        public int WordCount() => text.Split(' ', StringSplitOptions.RemoveEmptyEntries).Length;

        public bool IsBlank => string.IsNullOrWhiteSpace(text); // an extension property
    }

    public static T SecondLargest<T>(this IEnumerable<T> source) => // the older form
        source.OrderDescending().Skip(1).First();
}
```

```output for=extension-block-and-property
3 False True
2 2
```

- Verdict: native. C# 14 added `extension` blocks, which can hold extension methods and extension properties for one receiver. `IsBlank` is a property, so code writes `s.IsBlank` with no parentheses.
- `SecondLargest` uses the older form, with `this` on the first parameter, and it works for every `IEnumerable<T>`. Both forms compile to static methods, so `TextExtensions.WordCount("one two")` is a valid call too.
