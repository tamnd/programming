```csharp run id=round-trip-format
using System;
using System.Globalization;
using System.Xml;

var inv = CultureInfo.InvariantCulture;
var t = new DateTimeOffset(2024, 7, 9, 14, 30, 5, 120, TimeSpan.FromHours(2));
// "o" is the round-trip format: ISO 8601 with seven digits of fraction.
Console.WriteLine(t.ToString("o", inv));
Console.WriteLine(t.UtcDateTime.ToString("o", inv));
Console.WriteLine(DateTimeOffset.Parse("2024-07-09T12:30:05.12Z", inv) == t);

// ISOWeek gives the week-numbering year and the week. DayOfWeek counts Sunday as 0.
var d = new DateTime(2024, 12, 30);
Console.WriteLine($"{ISOWeek.GetYear(d)}-W{ISOWeek.GetWeekOfYear(d):00}-{((int)d.DayOfWeek + 6) % 7 + 1}");

// The XML Schema type duration uses the ISO 8601 form.
Console.WriteLine(XmlConvert.ToTimeSpan("PT1H30M"));
Console.WriteLine(XmlConvert.ToString(TimeSpan.FromMinutes(90)));
```

```output
2024-07-09T14:30:05.1200000+02:00
2024-07-09T12:30:05.1200000Z
True
2025-W01-1
01:30:00
PT1H30M
```

- Verdict: stdlib. The format specifier `"o"` writes ISO 8601, and `Parse` with the invariant culture reads it. `System.Globalization.ISOWeek` computes week dates. `XmlConvert` reads and writes durations in the form of ISO 8601, which XML Schema uses.
