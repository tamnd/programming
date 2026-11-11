```csharp run id=gcd-top-level
static long Gcd(long a, long b)
{
    while (b != 0)
    {
        (a, b) = (b, a % b);
        Console.WriteLine($"{a} {b}");
    }
    return a;
}

Console.WriteLine("1071 462");
Console.WriteLine($"gcd {Gcd(1071, 462)}");
return 0;
```

```output for=gcd-top-level
1071 462
462 147
147 21
21 0
gcd 21
```

- Verdict: native. A C# program is an assembly with one entry point. With top-level statements, the statements of one file form the body of the `Main` method that the compiler writes, so the file can start with code, as here. A tuple assignment updates both variables at once, and the value of `return` in the top-level code is the exit status.
