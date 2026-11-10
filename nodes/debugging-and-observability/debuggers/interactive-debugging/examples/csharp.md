```csharp run id=stacktrace
using System.Diagnostics;

Console.WriteLine($"total: {Shop.Total(["apple", "bread"])}");

// Debugger.Break() would stop here under a debugger. Without one, the program
// checks first, because Break can start a dialog or a crash report.
Console.WriteLine($"debugger attached: {Debugger.IsAttached}");
if (Debugger.IsAttached)
{
    Debugger.Break();
}

static class Shop
{
    public static int Total(string[] cart) => cart.Sum(Price);

    public static int Price(string item)
    {
        if (item == "bread")
        {
            Console.WriteLine("  " + string.Join(" <- ", WhereAmI()));
            return 250;
        }
        return 30;
    }

    // The frames of this class on the stack, innermost first.
    static IEnumerable<string> WhereAmI() =>
        new StackTrace(1) // skip WhereAmI itself
            .GetFrames()
            .Select(DiagnosticMethodInfo.Create)
            .Where(method => method?.DeclaringTypeName == "Shop")
            .Select(method => method!.Name);
}
```

```output for=stacktrace
  Price <- Total
total: 280
debugger attached: False
```

- Verdict: library. The debugger of .NET is a tool outside the program, such as the debugger of an IDE. `System.Diagnostics.Debugger` lets the code talk to it.
- `Debugger.Break` signals a breakpoint to an attached debugger. When no debugger is attached, it can ask the user to attach one, so the example checks `Debugger.IsAttached` first.
- `StackTrace` gives the frames of the current thread. `DiagnosticMethodInfo` gives the name of the method of a frame for logs.
