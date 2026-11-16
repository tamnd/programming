```csharp run id=local-function-csharp
using System;

class Program
{
    static int SumOfScaled(int[] values, int factor)
    {
        int total = 0;
        foreach (int v in values)
        {
            total += Scale(v);
        }
        return total;

        int Scale(int x)
        {
            return x * factor;
        }
    }

    static void Main()
    {
        Console.WriteLine(SumOfScaled(new[] { 1, 2, 3 }, 10));
        Console.WriteLine(SumOfScaled(new[] { 4 }, 2));
    }
}
```

```output for=local-function-csharp
60
8
```

- Verdict: native. `Scale` is a local function. It captures the parameter `factor` of the containing method.
