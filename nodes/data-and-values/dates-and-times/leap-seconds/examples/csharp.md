```csharp run id=no-second-sixty
using System;
using System.Globalization;

var inv = CultureInfo.InvariantCulture;
// DateTimeOffset counts seconds with no leap seconds, so the second 60 does not parse.
Console.WriteLine(DateTimeOffset.TryParse("2016-12-31T23:59:60Z", inv, DateTimeStyles.None, out _));
try
{
    new DateTime(2016, 12, 31, 23, 59, 60);
}
catch (ArgumentOutOfRangeException e)
{
    Console.WriteLine(e.GetType().Name);
}

var a = new DateTimeOffset(2016, 12, 31, 23, 59, 59, TimeSpan.Zero);
var b = new DateTimeOffset(2017, 1, 1, 0, 0, 0, TimeSpan.Zero);
Console.WriteLine((b - a).TotalSeconds);
Console.WriteLine(b.ToUnixTimeSeconds() - a.ToUnixTimeSeconds());
```

```output
False
ArgumentOutOfRangeException
1
1
```

- Verdict: stdlib. `DateTime` and `DateTimeOffset` have 86400 seconds in each day, and `ToUnixTimeSeconds` "does not take leap seconds into account". On Linux and macOS the parsers reject the second 60, and the constructors throw.
