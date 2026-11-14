```csharp run id=to-unix-time-seconds
using System;

// DateTimeOffset converts to and from a Unix time in seconds or milliseconds.
var t = DateTimeOffset.FromUnixTimeSeconds(1709985600);
Console.WriteLine(t.ToString("O"));
Console.WriteLine(t.ToUnixTimeSeconds() + " " + t.ToUnixTimeMilliseconds());
Console.WriteLine(DateTimeOffset.UnixEpoch.ToString("O"));

// The same instant with a different offset has the same Unix time.
var tokyo = new DateTimeOffset(2024, 3, 9, 21, 0, 0, TimeSpan.FromHours(9));
Console.WriteLine(tokyo.ToUnixTimeSeconds());

// DateTime counts ticks of 100 ns from the year 1, not from 1970.
Console.WriteLine(DateTime.UnixEpoch.Ticks);
```

```output
2024-03-09T12:00:00.0000000+00:00
1709985600 1709985600000
1970-01-01T00:00:00.0000000+00:00
1709985600
621355968000000000
```

- Verdict: stdlib. DateTimeOffset.FromUnixTimeSeconds, FromUnixTimeMilliseconds, ToUnixTimeSeconds and ToUnixTimeMilliseconds convert in both directions, and DateTime.UnixEpoch and DateTimeOffset.UnixEpoch are the epoch. The native count of DateTime is ticks since 0001-01-01.
