```csharp run id=culture-info
using System;
using System.Globalization;

double x = 1234567.891;
foreach (var name in new[] { "en-US", "de-DE", "en-IN", "de-CH" })
{
    var c = CultureInfo.GetCultureInfo(name);
    Console.WriteLine($"{name} {x.ToString("N2", c)}");
}

var inv = CultureInfo.InvariantCulture;
Console.WriteLine(x.ToString("N2", inv) + " " + x.ToString(inv));
Console.WriteLine(string.Format(CultureInfo.GetCultureInfo("de-DE"), "{0:F1}", 2.5));
var nf = CultureInfo.GetCultureInfo("en-IN").NumberFormat;
Console.WriteLine(string.Join(",", nf.NumberGroupSizes));
Console.WriteLine(double.Parse("1.234,5", CultureInfo.GetCultureInfo("de-DE")).ToString(inv));
```

```output for=culture-info
en-US 1,234,567.89
de-DE 1.234.567,89
en-IN 12,34,567.89
de-CH 1’234’567.89
1,234,567.89 1234567.891
2,5
3,2
1234.5
```

- Verdict: stdlib. A `CultureInfo` holds a `NumberFormatInfo` with the separators and the group sizes, and `ToString`, `string.Format` and `double.Parse` take it as an argument. Without one, they use the current culture of the thread. `CultureInfo.InvariantCulture` is the neutral culture for machine text. On Linux, .NET reads the culture data from ICU.
