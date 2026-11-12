```csharp run id=timespan-and-unit-structs
using System;

var timeout = TimeSpan.FromSeconds(90) + TimeSpan.FromMilliseconds(500);
Console.WriteLine($"{timeout} = {timeout.TotalMilliseconds} ms");
Console.WriteLine($"2 min / 30 s = {TimeSpan.FromMinutes(2) / TimeSpan.FromSeconds(30)}");

var trip = new Metres(5000) + new Feet(1000).ToMetres();
var speed = trip / new Seconds(1800);
Console.WriteLine($"{trip}, {speed}");

readonly record struct Metres(double Value)
{
    public static Metres operator +(Metres a, Metres b) => new(a.Value + b.Value);
    public static MetresPerSecond operator /(Metres d, Seconds t) => new(d.Value / t.Value);
    public override string ToString() => $"{Value:0.##} m";
}

readonly record struct Feet(double Value)
{
    public Metres ToMetres() => new(Value * 0.3048);
}

readonly record struct Seconds(double Value);

readonly record struct MetresPerSecond(double Value)
{
    public override string ToString() => $"{Value:0.###} m/s";
}
```

```output for=timespan-and-unit-structs
00:01:30.5000000 = 90500 ms
2 min / 30 s = 4
5304.8 m, 2.947 m/s
```

```csharp fails expect=compile id=metres-plus-feet
using System;

var height = new Metres(1000) + new Feet(500);
Console.WriteLine(height);

readonly record struct Metres(double Value)
{
    public static Metres operator +(Metres a, Metres b) => new(a.Value + b.Value);
}

readonly record struct Feet(double Value);
```

- Verdict: library. `TimeSpan` is the unit type for time. Record structs with operators model other units: `Metres / Seconds` gives `MetresPerSecond`, because the operator says so, and the structs cost no more than a `double`. The fails block adds `Metres` and `Feet`, and the compiler reports CS0019 because no `+` operator takes both. The UnitsNet library defines several thousand units in this style.
