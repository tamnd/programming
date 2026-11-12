```csharp run id=wrappers
using System;

interface IThermometer { int Read(); }

class Sensor : IThermometer
{
    public int Reads;
    public int Read() { Reads++; return 20; }
}

class FahrenheitAdapter
{
    private readonly IThermometer sensor;
    public FahrenheitAdapter(IThermometer sensor) { this.sensor = sensor; }
    public int Fahrenheit() => sensor.Read() * 9 / 5 + 32;
}

class Calibrated : IThermometer
{
    private readonly IThermometer sensor;
    private readonly int offset;
    public Calibrated(IThermometer sensor, int offset) { this.sensor = sensor; this.offset = offset; }
    public int Read() => sensor.Read() + offset;
}

class CachingProxy : IThermometer
{
    private readonly Lazy<int> value;
    public CachingProxy(IThermometer sensor) { value = new Lazy<int>(sensor.Read); }
    public int Read() => value.Value;
}

class Program
{
    static void Main()
    {
        Console.WriteLine($"adapter: {new FahrenheitAdapter(new Sensor()).Fahrenheit()} F");
        Console.WriteLine($"decorator: {new Calibrated(new Sensor(), 1).Read()}");
        var real = new Sensor();
        IThermometer proxy = new CachingProxy(real);
        Console.WriteLine($"proxy: {proxy.Read()} {proxy.Read()}, real reads: {real.Reads}");
    }
}
```

```output
adapter: 68 F
decorator: 21
proxy: 20 20, real reads: 1
```

- Verdict: idiom. The wrappers are classes that implement the interface of the sensor. `Lazy<T>` holds the one real read of the proxy.
