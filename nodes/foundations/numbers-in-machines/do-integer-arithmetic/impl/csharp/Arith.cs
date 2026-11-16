// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-integer-arithmetic/.
public static class Arithmetic
{
    // Returns a + b, a - b, a * b, a / b (rounded toward zero), a % b (with
    // the sign of a) and a to the power n. C# has no power operator, and
    // Math.Pow works on double values, so a loop keeps the power exact.
    public static long[] Arith(long a, long b, int n)
    {
        long power = 1;
        for (int i = 0; i < n; i++)
        {
            power *= a;
        }
        return new[] { a + b, a - b, a * b, a / b, a % b, power };
    }
}
