```csharp run id=function-signature-csharp
using System;
using System.Globalization;

class Program
{
    static double Average(int a, int b)
    {
        return (a + b) / 2.0;
    }

    static void Main()
    {
        Console.WriteLine(Average(3, 4).ToString(CultureInfo.InvariantCulture));
    }
}
```

```output for=function-signature-csharp
3.5
```

- Verdict: native. The return type comes before the name, and each parameter has a type.
