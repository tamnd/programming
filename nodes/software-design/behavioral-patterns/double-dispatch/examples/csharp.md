```csharp run id=collisions
using System;

interface IBody
{
    string Name { get; }
    string Collide(IBody other);
    string CollideWithShip(Ship ship);
    string CollideWithAsteroid(Asteroid asteroid);
}

class Ship : IBody
{
    public string Name => "ship";
    public string Collide(IBody other) => other.CollideWithShip(this);
    public string CollideWithShip(Ship ship) => "ships bounce";
    public string CollideWithAsteroid(Asteroid asteroid) => "ship takes damage";
}

class Asteroid : IBody
{
    public string Name => "asteroid";
    public string Collide(IBody other) => other.CollideWithAsteroid(this);
    public string CollideWithShip(Ship ship) => "ship takes damage";
    public string CollideWithAsteroid(Asteroid asteroid) => "asteroids break apart";
}

class Program
{
    static void Main()
    {
        var pairs = new (IBody, IBody)[]
        {
            (new Ship(), new Asteroid()),
            (new Asteroid(), new Ship()),
            (new Asteroid(), new Asteroid()),
            (new Ship(), new Ship()),
        };
        foreach (var (a, b) in pairs)
        {
            Console.WriteLine($"{a.Name} + {b.Name}: {a.Collide(b)}");
        }
    }
}
```

```output
ship + asteroid: ship takes damage
asteroid + ship: ship takes damage
asteroid + asteroid: asteroids break apart
ship + ship: ships bounce
```

- Verdict: idiom. This is the classic form with an interface. C# can also cast the argument to `dynamic`, which gives multiple dispatch without the callbacks.
