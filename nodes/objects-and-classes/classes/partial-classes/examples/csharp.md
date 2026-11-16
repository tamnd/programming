```csharp run id=partial-property-interface-struct
using System;

var p = new Person { Name = "  Ada " };
Console.WriteLine($"[{p.Name}]");
IShape s = new Square(3);
Console.WriteLine($"{s.Area()} {s.Name()}");

partial class Person
{
    public partial string Name { get; set; } // the declaration
}

partial class Person
{
    private string name = "";

    public partial string Name // the implementation, for example from a source generator
    {
        get => name;
        set => name = value.Trim();
    }
}

partial interface IShape
{
    double Area();
}

partial interface IShape
{
    string Name();
}

partial struct Square : IShape
{
    readonly double side;

    public Square(double side) => this.side = side;
}

partial struct Square
{
    public double Area() => side * side;

    public string Name() => "square";
}
```

```output for=partial-property-interface-struct
[Ada]
9 square
```

- Verdict: native. Classes, structs, interfaces and records can be `partial`, and so can members: here one part declares the property `Name` and the other part implements it.
- The interface `IShape` has its two methods in two parts, and the struct `Square` names the interface in one part and implements it in the other. The compiler checks the joined type, so a missing method is still an error.
