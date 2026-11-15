// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-class/.

/// <summary>A rectangle with a width and a height.</summary>
public sealed class Rectangle
{
    // Properties with a private setter: other code can read them, and only
    // the methods of this class can change them.
    public int Width { get; private set; }
    public int Height { get; private set; }

    public Rectangle(int width, int height)
    {
        Width = width;
        Height = height;
    }

    public int Area() => Width * Height;

    public int Perimeter() => 2 * (Width + Height);

    /// <summary>Multiplies both sides by factor. This changes the instance.</summary>
    public void Scale(int factor)
    {
        Width *= factor;
        Height *= factor;
    }

    public override string ToString() => $"Rectangle({Width}, {Height})";

    public static int[] Report(int width, int height, int factor)
    {
        var r = new Rectangle(width, height);
        int area = r.Area();
        int perimeter = r.Perimeter();
        r.Scale(factor);
        return [area, perimeter, r.Area(), r.Perimeter()];
    }
}
