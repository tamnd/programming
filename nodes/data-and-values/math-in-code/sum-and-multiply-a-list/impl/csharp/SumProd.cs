// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sum-and-multiply-a-list/.

using System.Linq;

/// <summary>The sum and the product of a list of long values.</summary>
public static class SumProd
{
    /// <summary>Returns the sum and the product. Both throw OverflowException on overflow.</summary>
    public static (long Sum, long Product) SumAndProduct(long[] items)
    {
        long sum = items.Sum();
        long product = items.Aggregate(1L, (a, x) => checked(a * x));
        return (sum, product);
    }
}
