// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

using System.Numerics;

/// <summary>Complex number arithmetic with System.Numerics.Complex.</summary>
public static class ComplexArith
{
    /// <summary>Returns {sum, product, quotient, abs(a), phase(a)} as 8 numbers. a and b are {re, im}.</summary>
    public static double[] ComplexArithmetic(double[] a, double[] b)
    {
        var z = new Complex(a[0], a[1]);
        var w = new Complex(b[0], b[1]);
        Complex s = z + w, p = z * w, q = z / w;
        return [s.Real, s.Imaginary, p.Real, p.Imaginary, q.Real, q.Imaginary, z.Magnitude, z.Phase];
    }
}
