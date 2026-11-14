```csharp run id=add-months
using System;
using System.Globalization;

var inv = CultureInfo.InvariantCulture;
// AddMonths clamps the day to the last valid day of the new month.
var jan31 = new DateTime(2024, 1, 31);
Console.WriteLine(jan31.AddMonths(1).ToString("yyyy-MM-dd", inv));
Console.WriteLine(jan31.AddMonths(1).AddMonths(1).ToString("yyyy-MM-dd", inv) + " " + jan31.AddMonths(2).ToString("yyyy-MM-dd", inv));
Console.WriteLine(new DateTime(2024, 2, 29).AddYears(1).ToString("yyyy-MM-dd", inv));

// A DateTime has no zone, so AddDays adds a day of wall-clock time.
var ny = TimeZoneInfo.FindSystemTimeZoneById("America/New_York");
var local = new DateTime(2024, 3, 9, 18, 0, 0);
var next = local.AddDays(1);
Console.WriteLine(next.ToString("yyyy-MM-dd HH:mm", inv) + " " + ny.GetUtcOffset(next));

// Exact arithmetic: add 24 hours to the instant, then convert to the zone.
var start = new DateTimeOffset(local, ny.GetUtcOffset(local));
var exact = TimeZoneInfo.ConvertTime(start + TimeSpan.FromHours(24), ny);
Console.WriteLine(exact.ToString("yyyy-MM-dd HH:mm zzz", inv));
```

```output
2024-02-29
2024-03-29 2024-03-31
2025-02-28
2024-03-10 18:00 -04:00:00
2024-03-10 19:00 -04:00
```

- Verdict: stdlib. `AddMonths` and `AddYears` clamp the day to the last valid day of the month. A `DateTime` has no zone, so `AddDays` adds wall-clock time. For exact durations, add a `TimeSpan` to a `DateTimeOffset` and convert the result with `TimeZoneInfo.ConvertTime`.
