```csharp run id=composite-formatting
using System;
using System.Globalization;

CultureInfo.CurrentCulture = CultureInfo.InvariantCulture;
double x = 3.14159;
Console.WriteLine("[{0,8:F2}] [{0,-8:F2}] [{0:00000.00}] [{0:+0.0;-0.0}]", x);
Console.WriteLine("[{0,6}] [{0,-6}] [{1:N0}] [{2:X}] [{2:X4}]", "abc", 1234567, 255);
Console.WriteLine("[{0:E2}] [{1:P0}] [{2:D5}]", 1234.5, 0.25, 42);
Console.WriteLine($"[{x,8:F2}] [{{x}}]");
```

```output for=composite-formatting
[    3.14] [3.14    ] [00003.14] [+3.1]
[   abc] [abc   ] [1,234,567] [FF] [00FF]
[1.23E+003] [25 %] [00042]
[    3.14] [{x}]
```

- Verdict: stdlib. A format item is `{index,width:format}`. A negative width aligns on the left. The format is a standard string such as `F2`, `N0`, `X4`, `E2`, `P0` or `D5`, or a custom pattern such as `00000.00`. The output depends on the current culture, so the program sets the invariant culture. Interpolated strings use the same items.
