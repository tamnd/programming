```csharp run id=move
using System;

var servant = new MoveServant();
Shape[] shapes = { new Rectangle(new(0, 0)), new Ellipse(new(10, 0)), new Triangle(new(0, 10)) };
foreach (var s in shapes)
{
    servant.MoveBy(s, 3, 4);
    Console.WriteLine(s);
}
servant.MoveTo(shapes[2], new(1, 1));
Console.WriteLine(shapes[2]);

record Position(int X, int Y);

interface IMovable
{
    Position Position { get; set; }
}

class MoveServant
{
    public void MoveTo(IMovable serviced, Position where) => serviced.Position = where;

    public void MoveBy(IMovable serviced, int dx, int dy) =>
        serviced.Position = new Position(serviced.Position.X + dx, serviced.Position.Y + dy);
}

abstract class Shape : IMovable
{
    private readonly string name;
    protected Shape(string name, Position p) { this.name = name; Position = p; }
    public Position Position { get; set; }
    public override string ToString() => $"{name} at ({Position.X}, {Position.Y})";
}

class Rectangle : Shape { public Rectangle(Position p) : base("rectangle", p) { } }
class Ellipse : Shape { public Ellipse(Position p) : base("ellipse", p) { } }
class Triangle : Shape { public Triangle(Position p) : base("triangle", p) { } }
```

```output for=move
rectangle at (3, 4)
ellipse at (13, 4)
triangle at (3, 14)
triangle at (1, 1)
```

- Verdict: idiom. The interface `IMovable` states what the servant needs. Here it is a property, not two methods.
