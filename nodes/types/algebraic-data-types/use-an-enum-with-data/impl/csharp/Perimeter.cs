// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-an-enum-with-data/.
using System.Collections.Generic;
using System.Linq;

// An abstract record with a private constructor: only the nested records
// below can derive from it, so they are all of its variants.
public abstract record Shape
{
    private Shape() { }

    public sealed record Square(int Side) : Shape;

    public sealed record Rectangle(int Width, int Height) : Shape;

    public sealed record Triangle(int A, int B, int C) : Shape;
}

public static class Perimeter
{
    public static Shape? MakeShape(string kind, IReadOnlyList<int> sides)
    {
        if (sides.Any(s => s <= 0))
        {
            return null;
        }
        return (kind, sides.Count) switch
        {
            ("square", 1) => new Shape.Square(sides[0]),
            ("rectangle", 2) => new Shape.Rectangle(sides[0], sides[1]),
            ("triangle", 3) when IsTriangle(sides[0], sides[1], sides[2]) =>
                new Shape.Triangle(sides[0], sides[1], sides[2]),
            _ => null,
        };
    }

    private static bool IsTriangle(int a, int b, int c) => a < b + c && b < a + c && c < a + b;

    // The compiler cannot see that the list of variants is closed, so the
    // switch needs a discard arm.
    public static int ShapePerimeter(Shape shape) => shape switch
    {
        Shape.Square(var side) => 4 * side,
        Shape.Rectangle(var width, var height) => 2 * (width + height),
        Shape.Triangle(var a, var b, var c) => a + b + c,
        _ => throw new System.ArgumentException("unknown shape", nameof(shape)),
    };

    public static int? Of(string kind, IReadOnlyList<int> sides) =>
        MakeShape(kind, sides) is { } shape ? ShapePerimeter(shape) : null;
}
