// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-big-integers/.
using System.Globalization;
using System.Numerics;

public static class BigMath
{
    // Returns base to the power exp as decimal text. A BigInteger has no fixed width.
    public static string BigPower(int @base, int exp)
    {
        return BigInteger.Pow(@base, exp).ToString(CultureInfo.InvariantCulture);
    }
}
