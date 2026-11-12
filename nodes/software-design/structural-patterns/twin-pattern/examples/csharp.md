```csharp run id=twin-ball
using System;

abstract class Item
{
    public readonly string Name;
    protected Item(string name) { Name = name; }
    public abstract int Position();
    public void Show() => Console.WriteLine($"{Name} at {Position()}");
}

abstract class Ticker
{
    public int Count;
    protected abstract void OnTick();
    public void Tick() { Count++; OnTick(); }
}

class BallItem : Item
{
    public int X;
    public readonly BallTicker Twin;
    public BallItem(string name) : base(name) { Twin = new BallTicker(this); }
    public override int Position() => X;
    public void Move() => X += 2;
}

class BallTicker : Ticker
{
    private readonly BallItem twin;
    public BallTicker(BallItem twin) { this.twin = twin; }
    protected override void OnTick() => twin.Move();
}

class Program
{
    static void Main()
    {
        var ball = new BallItem("ball");
        ball.Show();
        for (int i = 0; i < 2; i++)
        {
            ball.Twin.Tick();
            ball.Show();
        }
        Console.WriteLine($"ticks: {ball.Twin.Count}");
    }
}
```

```output
ball at 0
ball at 2
ball at 4
ticks: 2
```

- Verdict: idiom. A class has one base class and can implement several interfaces, so the code of a second parent needs a twin.
