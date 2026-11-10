```csharp run id=thermometer
using System.Globalization;

static void Report(string name, IThermometer sensor) =>
    Console.WriteLine($"{name}: {sensor.Celsius().ToString("F1", CultureInfo.InvariantCulture)} C");

Report("lab", new RoomSensor(21.5));
Report("patient", new LegacyAdapter(new LegacyThermometer(986)));
Report("hall", new LegacyAdapter(new LegacyThermometer(704)));

interface IThermometer
{
    double Celsius();
}

sealed class RoomSensor(double value) : IThermometer
{
    public double Celsius() => value;
}

// The vendor class. The program cannot change it.
sealed class LegacyThermometer(int tenths)
{
    public int FahrenheitTenths() => tenths;
}

sealed class LegacyAdapter(LegacyThermometer legacy) : IThermometer
{
    public double Celsius() => (legacy.FahrenheitTenths() / 10.0 - 32) * 5 / 9;
}
```

```output
lab: 21.5 C
patient: 37.0 C
hall: 21.3 C
```

- Verdict: idiom. The adapter implements the interface of the client and takes the adaptee through a primary constructor parameter.
- `CultureInfo.InvariantCulture` fixes the decimal point. The culture of the machine can print a comma.
- `System.IO.StreamReader` is an adapter in the class library: it gives a `TextReader` over a byte `Stream`.
- A `Func<double>` can replace the interface when the client needs only one call, and a lambda is then the adapter.
