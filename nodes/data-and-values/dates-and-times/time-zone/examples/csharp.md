```csharp run id=time-zone-info
using System;
using System.Globalization;

var ny = TimeZoneInfo.FindSystemTimeZoneById("America/New_York");
const string f = "yyyy-MM-dd'T'HH:mmzzz";
var winter = new DateTimeOffset(2024, 1, 9, 12, 0, 0, TimeSpan.Zero);
var summer = new DateTimeOffset(2024, 7, 9, 12, 0, 0, TimeSpan.Zero);
Console.WriteLine(TimeZoneInfo.ConvertTime(winter, ny).ToString(f, CultureInfo.InvariantCulture));
Console.WriteLine(TimeZoneInfo.ConvertTime(summer, ny).ToString(f, CultureInfo.InvariantCulture));

// A gap and an overlap.
Console.WriteLine(ny.IsInvalidTime(new DateTime(2024, 3, 10, 2, 30, 0)));
var overlap = new DateTime(2024, 11, 3, 1, 30, 0);
Console.WriteLine(ny.IsAmbiguousTime(overlap));
Console.WriteLine(string.Join(" ", ny.GetAmbiguousTimeOffsets(overlap)));
```

```output
2024-01-09T07:00-05:00
2024-07-09T08:00-04:00
True
True
-05:00:00 -04:00:00
```

- Verdict: stdlib. TimeZoneInfo represents a zone with its adjustment rules, and FindSystemTimeZoneById accepts IANA names on all platforms. IsInvalidTime and IsAmbiguousTime find gaps and overlaps, and DateTimeOffset holds one instant with one offset.
