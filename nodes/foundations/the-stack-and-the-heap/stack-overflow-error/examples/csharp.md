```csharp run id=deep
using System;

static class Demo
{
    const int Limit = 10_000; // well below the depth where the stack runs out

    // Returns -1 when the depth passes Limit, so the stack never overflows.
    static int Depth(int n, int level)
    {
        if (level > Limit) return -1;
        if (n == 0) return 0;
        int rest = Depth(n - 1, level + 1);
        return rest < 0 ? rest : 1 + rest;
    }

    static int Loop(int n)
    {
        int count = 0;
        while (n > 0) // one frame for the whole loop
        {
            n--;
            count++;
        }
        return count;
    }

    static void Main()
    {
        Console.WriteLine($"depth(100) = {Depth(100, 1)}");
        if (Depth(1_000_000, 1) < 0)
        {
            Console.WriteLine($"depth(1000000): stopped at the limit of {Limit}");
        }
        Console.WriteLine($"loop(1000000) = {Loop(1_000_000)}");
    }
}
```

```output for=deep
depth(100) = 100
depth(1000000): stopped at the limit of 10000
loop(1000000) = 1000000
```

- Verdict: native. A stack overflow throws `StackOverflowException`, but a `try`/`catch` cannot catch it, and the process ends. The program counts its own depth and stops first, as the .NET documentation advises.
