// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-integer-overflow/.
using System;

public static class Overflow
{
    // Returns a + b, or null when the sum does not fit in a long. In a checked
    // context an overflow throws OverflowException. The default context is
    // unchecked, and there a + b wraps.
    public static long? CheckedAdd(long a, long b)
    {
        try
        {
            return checked(a + b);
        }
        catch (OverflowException)
        {
            return null;
        }
    }
}
