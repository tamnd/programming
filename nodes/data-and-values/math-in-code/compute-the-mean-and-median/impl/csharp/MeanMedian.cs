// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-mean-and-median/.

using System.Linq;

/// <summary>The mean and the median of a list of long values.</summary>
public static class MeanMedian
{
    /// <summary>Returns the mean and the median of a non-empty array. The array does not change.</summary>
    public static (double Mean, double Median) MeanAndMedian(long[] items)
    {
        // The selector makes the sum a double, so it does not overflow.
        double mean = items.Average(x => (double)x);
        long[] s = items.Order().ToArray();
        int mid = s.Length / 2;
        double median = s.Length % 2 == 1 ? s[mid] : ((double)s[mid - 1] + s[mid]) / 2;
        return (mean, median);
    }
}
