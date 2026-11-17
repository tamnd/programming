```csharp run id=frames
using System;

static class Demo
{
    static void Count(int n)
    {
        int here = n * 10; // a local: one copy in each call
        Console.WriteLine($"enter n={n} here={here}");
        if (n > 1)
        {
            Count(n - 1);
        }
        Console.WriteLine($"leave n={n} here={here}");
    }

    static void Main()
    {
        Count(3);
    }
}
```

```output for=frames
enter n=3 here=30
enter n=2 here=20
enter n=1 here=10
leave n=1 here=10
leave n=2 here=20
leave n=3 here=30
```

- Verdict: native. Each call has its own copy of `n` and `here`. Where the runtime keeps them is its own choice, so the program should depend only on the copy for each call.
- A local that a lambda captures moves into a hidden object on the heap, so it lives after the call returns.
