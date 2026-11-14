```csharp run id=datetimeoffset-and-dateonly
using System;

// A DateTimeOffset is a date and time with an offset from UTC, so it is an instant.
var utc = new DateTimeOffset(2024, 3, 9, 12, 0, 0, TimeSpan.Zero);
var tokyo = utc.ToOffset(TimeSpan.FromHours(9));
Console.WriteLine(utc.ToString("O") + " " + tokyo.ToString("O") + " " + (utc == tokyo));
Console.WriteLine(utc.ToUnixTimeSeconds());

// A DateTime without a kind is not tied to a zone.
var wall = new DateTime(2024, 3, 9, 12, 0, 0);
Console.WriteLine(wall.Kind);

// A DateOnly has no time. Plus 1 month clamps to the end of February.
var jan31 = new DateOnly(2024, 1, 31);
Console.WriteLine(jan31.AddMonths(1).ToString("yyyy-MM-dd"));

// A TimeSpan is an amount of time.
Console.WriteLine(TimeSpan.FromMinutes(90) + " " + TimeSpan.FromMinutes(90).TotalSeconds);
```

```output
2024-03-09T12:00:00.0000000+00:00 2024-03-09T21:00:00.0000000+09:00 True
1709985600
Unspecified
2024-02-29
01:30:00 5400
```

- Verdict: stdlib. .NET has DateTime, DateTimeOffset, DateOnly, TimeOnly, TimeSpan and TimeZoneInfo. A DateTimeOffset names one instant. A DateTime has a Kind of Utc, Local or Unspecified, and two values with different kinds compare by their fields only.
