// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-division-by-zero/.
using System;

public static class Division
{
    // Returns a / b rounded toward zero, or null for a zero divisor or a
    // quotient that does not fit in a long. Integer division by zero throws
    // DivideByZeroException, and long.MinValue / -1 throws an
    // ArithmeticException, even in an unchecked context. Catch both.
    public static long? SafeDiv(long a, long b)
    {
        try
        {
            return a / b;
        }
        catch (ArithmeticException)
        {
            return null;
        }
    }
}
