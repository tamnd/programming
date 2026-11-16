// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/return-multiple-values/.

/// <summary>Floor division that returns two results in a tuple.</summary>
public static class Division
{
    /// <summary>Returns the quotient and the remainder of the floor division of a by b.</summary>
    public static (int Quotient, int Remainder) DivMod(int a, int b)
    {
        int q = a / b;
        int r = a % b;
        if (r != 0 && (r < 0) != (b < 0))
        {
            q -= 1;
            r += b;
        }
        return (q, r);
    }
}
