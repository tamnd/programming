```csharp run id=throw-expressions
using System;
using System.Diagnostics.CodeAnalysis;

string host = Environment.GetEnvironmentVariable("PROG_EXAMPLE_NO_SUCH_VAR") ?? "";
try
{
    string h = host.Length > 0 ? host : throw new ArgumentException("no host");
    Console.WriteLine(h);
}
catch (ArgumentException e)
{
    Console.WriteLine(e.Message);
}

Console.WriteLine(Days.Name(1));
try
{
    Days.Name(9);
}
catch (ArgumentOutOfRangeException e)
{
    Console.WriteLine(e.ParamName);
}

try
{
    Days.Port("x");
}
catch (FormatException e)
{
    Console.WriteLine(e.Message);
}

static class Days
{
    public static string Name(int d) => d switch
    {
        1 => "Mon",
        2 => "Tue",
        _ => throw new ArgumentOutOfRangeException(nameof(d)),
    };

    [DoesNotReturn]
    static void Fail(string msg) => throw new FormatException(msg);

    public static int Port(string s)
    {
        if (int.TryParse(s, out int n))
        {
            return n;
        }
        Fail("bad port: " + s);
        return 0; // still needed: DoesNotReturn does not change the result type of Fail
    }
}
```

```output for=throw-expressions
no host
Mon
d
bad port: x
```

- Verdict: idiom. C# has no bottom type, but a `throw` expression can stand where a value is expected: in the conditional operator, after `??`, and in an arm of a `switch` expression. `DoesNotReturn` tells the nullable analysis that `Fail` never returns, but `Fail` still has the type `void`, so `Port` needs a `return` after the call.
