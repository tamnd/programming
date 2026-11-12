```csharp run id=duration
using System;
using System.Text.RegularExpressions;

sealed class Duration
{
    private static readonly Duration Zero = new Duration(0);
    private readonly long seconds;

    private Duration(long seconds) => this.seconds = seconds;

    public static Duration OfSeconds(long seconds) =>
        seconds == 0 ? Zero : new Duration(seconds); // a cached instance

    public static Duration OfMinutes(long minutes) => OfSeconds(minutes * 60);

    public static bool TryParse(string text, out Duration? result)
    {
        Match m = Regex.Match(text, @"^(\d+)([sm])$");
        if (!m.Success)
        {
            result = null; // a constructor cannot report "no object"
            return false;
        }
        long n = long.Parse(m.Groups[1].Value);
        result = OfSeconds(m.Groups[2].Value == "m" ? n * 60 : n);
        return true;
    }

    public override string ToString() => $"{seconds} s";
}

static class Program
{
    static void Main()
    {
        Console.WriteLine($"OfMinutes(2) = {Duration.OfMinutes(2)}");
        Console.WriteLine($"OfSeconds(90) = {Duration.OfSeconds(90)}");
        Console.WriteLine($"zero is cached: {ReferenceEquals(Duration.OfSeconds(0), Duration.OfMinutes(0))}");
        Console.WriteLine($"TryParse(\"2m\") = {(Duration.TryParse("2m", out var a) ? a : "failed")}");
        Console.WriteLine($"TryParse(\"2h\") = {(Duration.TryParse("2h", out var b) ? b : "failed")}");
    }
}
```

```output
OfMinutes(2) = 120 s
OfSeconds(90) = 90 s
zero is cached: True
TryParse("2m") = 120 s
TryParse("2h") = failed
```

- Verdict: idiom. A `private` constructor and `public static` methods. `TimeSpan.FromSeconds` and `TimeSpan.FromMinutes` in .NET have the same shape.
- The `TryParse` form returns `bool` and gives the object through an `out` parameter. It is the .NET convention for a factory that can fail without an exception.
