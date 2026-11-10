```csharp run id=interfaces timeout=30
IShape[] shapes = { new Square(2), new Circle(1) };
foreach (IShape shape in shapes)
{
    Console.WriteLine(shape.Describe());
}

Console.WriteLine(Sum(new[] { new Money(150), new Money(275) }));
Console.WriteLine(typeof(Square).GetInterfaces()[0].Name);

static T Sum<T>(T[] items) where T : IAddable<T>
{
    T total = T.Zero;
    foreach (T item in items)
    {
        total = total + item;
    }
    return total;
}

interface IShape
{
    double Area();
    string Describe() => $"{GetType().Name} {Area():F2}";
}

interface IAddable<T> where T : IAddable<T>
{
    static abstract T Zero { get; }
    static abstract T operator +(T left, T right);
}

class Square(double side) : IShape
{
    public double Area() => side * side;
}

class Circle(double radius) : IShape
{
    public double Area() => Math.PI * radius * radius;
    public string Describe() => $"circle of radius {radius}";
}

readonly record struct Money(long Cents) : IAddable<Money>
{
    public static Money Zero => new(0);
    public static Money operator +(Money left, Money right) => new(left.Cents + right.Cents);
}
```

```output
Square 4.00
circle of radius 1
Money { Cents = 425 }
IShape
```

- Verdict: native. A class, struct or record lists its interfaces after the colon. The relation is nominal.
- `Describe` in `IShape` has a body, so it is a default interface member. `Square` uses it. `Circle` declares its own public `Describe`, which implements the member.
- `IAddable<T>` has static abstract members. `Sum` calls `T.Zero` and `+` on a type parameter, and the compiler binds them for each type argument.
- A class does not inherit the default members of its interfaces. The Pitfalls section of the node shows the compile error.
