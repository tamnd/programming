```csharp run id=counter-module
using System;

static class Counter
{
    private static int privateCounter = 0;

    private static void ChangeBy(int val) => privateCounter += val;

    public static void Increment() => ChangeBy(1);
    public static void Decrement() => ChangeBy(-1);
    public static int Value() => privateCounter;
}

class Program
{
    static void Main()
    {
        Console.WriteLine($"value: {Counter.Value()}");
        Counter.Increment();
        Counter.Increment();
        Console.WriteLine($"value: {Counter.Value()}");
        Counter.Decrement();
        Console.WriteLine($"value: {Counter.Value()}");
    }
}
```

```output
value: 0
value: 2
value: 1
```

- Verdict: idiom. A `static class` cannot have instances, so it acts as a module. Namespaces group names but have no private members; `internal` limits a name to its assembly.
