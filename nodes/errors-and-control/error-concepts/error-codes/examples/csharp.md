```csharp run id=codes-csharp
using System;

static class Program
{
    const int Ok = 0, Empty = 1, NotANumber = 2, OutOfRange = 3;
    static readonly string[] Names = { "ok", "empty", "not a number", "out of range" };

    // Returns a code. On success, sets port.
    static int ParsePort(string text, out int port)
    {
        port = 0;
        if (text.Length == 0) return Empty;
        foreach (char c in text)
        {
            if (c < '0' || c > '9') return NotANumber;
        }
        if (text.Length > 5 || int.Parse(text) > 65535) return OutOfRange;
        port = int.Parse(text);
        return Ok;
    }

    static void Main()
    {
        foreach (string text in new[] { "8080", "", "x", "99999" })
        {
            int code = ParsePort(text, out int port);
            if (code != Ok)
            {
                Console.WriteLine($"'{text}' -> error code {code} ({Names[code]})");
            }
            else
            {
                Console.WriteLine($"'{text}' -> port {port}");
            }
        }
    }
}
```

```output for=codes-csharp
'8080' -> port 8080
'' -> error code 1 (empty)
'x' -> error code 2 (not a number)
'99999' -> error code 3 (out of range)
```

- Verdict: idiom. C# reports errors with exceptions, but each exception has an `HResult` property, "a coded numerical value that is assigned to a specific exception". An `out` parameter gives the method a second result.
