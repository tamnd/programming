// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-mathematical-constants-and-functions/.

using System;

/// <summary>Turns a distance and an angle in degrees into the coordinates x and y.</summary>
public static class Cartesian
{
    /// <summary>Returns the point at the distance r and the angle in degrees.</summary>
    public static (double X, double Y) ToCartesian(double r, double degrees)
    {
        double t = degrees * Math.PI / 180;
        return (r * Math.Cos(t), r * Math.Sin(t));
    }
}
