```csharp run id=car
using System;

interface IStarterPart { string Crank(); }

class Starter : IStarterPart { public string Crank() => "crank the engine"; }

class NewStarter : IStarterPart { public string Crank() => "crank the engine with the new starter"; }

class Car
{
    private sealed class Engine { public string Fire() => "fire the cylinders"; }
    private sealed class Wheels { public string Turn() => "turn the wheels"; }

    private IStarterPart starter = new Starter();
    private readonly Engine engine = new();
    private readonly Wheels wheels = new();

    public string Drive() => string.Join(", ", starter.Crank(), engine.Fire(), wheels.Turn());

    public void ReplaceStarter(IStarterPart starter) => this.starter = starter;
}

class Program
{
    static void Main()
    {
        var car = new Car();
        Console.WriteLine($"drive: {car.Drive()}");
        car.ReplaceStarter(new NewStarter());
        Console.WriteLine($"drive: {car.Drive()}");
    }
}
```

```output
drive: crank the engine, fire the cylinders, turn the wheels
drive: crank the engine with the new starter, fire the cylinders, turn the wheels
```

- Verdict: idiom. Private nested classes and private fields hide the parts, and an interface describes the part that can be replaced.
