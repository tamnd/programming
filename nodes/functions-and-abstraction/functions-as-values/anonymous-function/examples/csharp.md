```csharp run id=anonymous-function-csharp
using System;
using System.Collections.Generic;
using System.Linq;

class Program
{
    static void Main()
    {
        var words = new List<string> { "house", "car", "bike" };
        words.Sort((a, b) => a.Length.CompareTo(b.Length));
        Console.WriteLine(string.Join(" ", words));

        Console.WriteLine(string.Join(" ", new[] { 1, 2, 3 }.Select(x => x * x)));

        int n = 5;
        Func<int, int> add = x => x + n;
        Console.WriteLine(add(10));
    }
}
```

```output for=anonymous-function-csharp
car bike house
1 4 9
15
```

- Verdict: native. The lambdas use the operator `=>`. The lambda `add` captures the local variable `n`.
