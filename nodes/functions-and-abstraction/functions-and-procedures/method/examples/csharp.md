```csharp run id=method-csharp
using System;

struct Rectangle
{
    public int Width;
    public int Height;

    public Rectangle(int width, int height)
    {
        Width = width;
        Height = height;
    }

    public int Area()
    {
        return Width * Height;
    }

    public static Rectangle Square(int side)
    {
        return new Rectangle(side, side);
    }
}

class Program
{
    static void Main()
    {
        var r = new Rectangle(3, 4);
        Console.WriteLine(r.Area());
        Console.WriteLine(Rectangle.Square(5).Area());
    }
}
```

```output for=method-csharp
12
25
```

- Verdict: native. `Area` is an instance method of a struct, and `Square` is a `static` method that the program calls on the type.
