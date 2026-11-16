```csharp run id=const-and-readonly
using System;

Console.WriteLine(Limits.MaxSize);
var a = new Account(3);
Console.WriteLine(a.Id);

static class Limits
{
    public const int MaxSize = 10;
}

class Account
{
    public readonly int Id;

    public Account(int id)
    {
        Id = id;
    }
}
```

```output for=const-and-readonly
10
3
```

- Verdict: native. `const` gives a compile-time constant, and its value must be a constant expression. A `readonly` field gets its value in the declaration or in a constructor, so each object can have a different value, and an assignment after the constructor does not compile.
