```csharp run id=match-guards-csharp
using System;

static string Describe(int n) => n switch
{
    0 => "zero",
    < 0 => "negative",
    var k when k < 10 && k % 2 == 0 => "small even",
    < 10 => "small odd",
    _ => "large",
};

Console.WriteLine(string.Join(", ", Array.ConvertAll(new[] { -5, 0, 4, 7, 12 }, Describe)));
```

```output for=match-guards-csharp
negative, zero, small even, small odd, large
```

- Verdict: native. A switch expression arm can have a case guard after the `when` keyword. Relational patterns such as `< 0` and `< 10` need no guard. The arm for small even numbers needs one, because no pattern tests `k % 2`.
