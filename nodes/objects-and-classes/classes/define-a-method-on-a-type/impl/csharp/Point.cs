// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-method-on-a-type/.

using System;

/// <summary>A point on a grid. A readonly record struct is a value type with equality and ToString.</summary>
public readonly record struct Point(int X, int Y)
{
    public static readonly Point Origin = new(0, 0);

    /// <summary>Returns the point moved by (dx, dy).</summary>
    public Point Translate(int dx, int dy) => new(X + dx, Y + dy);

    /// <summary>Returns the grid distance |dx| + |dy| to another point.</summary>
    public int Manhattan(Point other) => Math.Abs(X - other.X) + Math.Abs(Y - other.Y);

    public static int[] Report(int x, int y, int dx, int dy)
    {
        var p = new Point(x, y);
        var q = p.Translate(dx, dy);
        return [q.X, q.Y, p.Manhattan(q), q.Manhattan(Origin)];
    }
}
