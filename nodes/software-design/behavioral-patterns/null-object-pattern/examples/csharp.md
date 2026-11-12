```csharp run id=null-logger
using System;

interface ILogger
{
    void Log(string message);
}

class ConsoleLogger : ILogger
{
    public void Log(string message) => Console.WriteLine($"log: {message}");
}

class NullLogger : ILogger
{
    public static readonly NullLogger Instance = new NullLogger();

    public void Log(string message) { } // do nothing
}

class Checkout
{
    private readonly ILogger logger;

    public Checkout(ILogger logger) => this.logger = logger;

    public string Pay(int amount)
    {
        logger.Log($"paid {amount}"); // no check for null
        return $"receipt: {amount}";
    }
}

class Program
{
    static void Main()
    {
        Console.WriteLine(new Checkout(new ConsoleLogger()).Pay(10));
        Console.WriteLine(new Checkout(NullLogger.Instance).Pay(20));
    }
}
```

```output
log: paid 10
receipt: 10
receipt: 20
```

- Verdict: idiom. `NullLogger` implements `ILogger` with an empty method. It has no state, so one shared instance is sufficient.
