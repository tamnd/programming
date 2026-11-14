```csharp run id=time-zone-info-history
using System;
using System.Globalization;

// On Linux and macOS, .NET reads the compiled tz files of the system.
var chicago = TimeZoneInfo.FindSystemTimeZoneById("America/Chicago");
foreach (var day in new[] { new DateTime(1880, 1, 1), new DateTime(1936, 6, 1), new DateTime(2024, 7, 1) })
{
    var t = new DateTimeOffset(day.AddHours(12), TimeSpan.Zero);
    var local = TimeZoneInfo.ConvertTime(t, chicago);
    Console.WriteLine(local.ToString("yyyy-MM-dd'T'HH:mm:ss", CultureInfo.InvariantCulture) + " " + local.Offset);
}

// Windows has its own zone IDs. .NET maps between them and the IANA names.
TimeZoneInfo.TryConvertIanaIdToWindowsId("America/Chicago", out string? windows);
Console.WriteLine(windows);
TimeZoneInfo.TryConvertWindowsIdToIanaId("Central Standard Time", out string? iana);
Console.WriteLine(iana);
```

```output
1880-01-01T06:09:00 -05:51:00
1936-06-01T07:00:00 -05:00:00
2024-07-01T07:00:00 -05:00:00
Central Standard Time
America/Chicago
```

- Verdict: stdlib. `TimeZoneInfo.FindSystemTimeZoneById` accepts IANA names on Linux and macOS, and since .NET 6 also on Windows when ICU is present. `TryConvertIanaIdToWindowsId` and `TryConvertWindowsIdToIanaId` use the mapping data of the Unicode CLDR. A `DateTimeOffset` stores its offset in whole minutes, so the offset -5:50:36 of Chicago in 1880 becomes -05:51, and the local time loses its 24 seconds.
