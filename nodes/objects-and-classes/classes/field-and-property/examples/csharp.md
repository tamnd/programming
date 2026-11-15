```csharp run id=properties
using System;

var r = new Rectangle(3, 4) { Name = "door" };
Console.WriteLine($"{r.Width} {r.Area} {r.Name}");
r.Width = 5;
Console.WriteLine(r.Area);
try
{
    r.Width = -1;
}
catch (ArgumentOutOfRangeException)
{
    Console.WriteLine("ArgumentOutOfRangeException");
}

class Rectangle
{
    private int width; // the backing field of Width

    public Rectangle(int width, int height)
    {
        Width = width;
        Height = height;
    }

    public int Width
    {
        get => width;
        set
        {
            ArgumentOutOfRangeException.ThrowIfNegative(value);
            width = value;
        }
    }

    public int Height { get; set; } // an auto-implemented property

    public string Name { get; init; } = ""; // set only during initialization

    public int Area => Width * Height; // a computed, read-only property
}
```

```output for=properties
3 12 door
20
ArgumentOutOfRangeException
```

- Verdict: native. A property has a `get` accessor, a `set` or `init` accessor, or both. Code reads and assigns it like a field, and the compiler turns each use into a call of the accessor.
- `Height { get; set; }` is an auto-implemented property: the compiler makes a hidden backing field. `Width` has an explicit backing field because its setter checks the value. `Area` has only an expression body, so it is computed on each read and cannot be assigned.
- `init` allows a value only in the constructor or in an object initializer, such as `{ Name = "door" }`.
