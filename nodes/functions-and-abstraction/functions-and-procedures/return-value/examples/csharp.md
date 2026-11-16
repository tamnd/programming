```csharp run id=return-value-csharp
using System;

class Program
{
    static int Absolute(int n)
    {
        if (n < 0)
        {
            return -n;
        }
        return n;
    }

    static void Main()
    {
        Console.WriteLine(Absolute(-7));
        Console.WriteLine(Absolute(5) + 1);
    }
}
```

```output for=return-value-csharp
7
6
```

- Verdict: native. The return type comes before the method name, and the compiler checks that each path ends with a `return`.
