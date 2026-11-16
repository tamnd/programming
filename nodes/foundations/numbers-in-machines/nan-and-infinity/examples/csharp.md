```csharp run id=traps
using System;
using System.Collections.Generic;
using System.Linq;
using System.Globalization;

CultureInfo.CurrentCulture = CultureInfo.InvariantCulture;
double nan = double.NaN;
Console.WriteLine($"nan == nan: {nan == nan}");
Console.WriteLine($"nan.Equals(nan): {nan.Equals(nan)}");
Console.WriteLine($"double.IsNaN(0.0 / 0.0): {double.IsNaN(0.0 / 0.0)}");
Console.WriteLine($"1.0 / 0.0: {1.0 / 0.0}, -1.0 / 0.0: {-1.0 / 0.0}");
Console.WriteLine($"IsNegative(-0.0): {double.IsNegative(-0.0)}");

double[] values = { 3.0, nan, 1.0, 2.0 };
Array.Sort(values);
Console.WriteLine($"Array.Sort: {string.Join(", ", values)}");

var set = new HashSet<double> { nan, 0.0 / 0.0 };
Console.WriteLine($"HashSet size: {set.Count}");
Console.WriteLine($"Math.Max(1.0, NaN): {Math.Max(1.0, nan)}");
Console.WriteLine($"Max of list: {new[] { 1.0, nan, 2.0 }.Max()}");
```

```output for=traps
nan == nan: False
nan.Equals(nan): True
double.IsNaN(0.0 / 0.0): True
1.0 / 0.0: Infinity, -1.0 / 0.0: -Infinity
IsNegative(-0.0): True
Array.Sort: NaN, 1, 2, 3
HashSet size: 1
Math.Max(1.0, NaN): NaN
Max of list: 2
```

- Verdict: native. C# `double` follows IEEE 754 for `==`.
- `Equals` and `CompareTo` treat NaN as equal to itself and smaller than every number, so `Array.Sort` puts NaN first and a `HashSet` keeps one NaN.
- `Math.Max` returns NaN, but the LINQ `Max` of a list skips it here, so two maximum functions give two answers.
