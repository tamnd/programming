// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-mean-and-median/.

import java.util.Arrays;
import java.util.stream.LongStream;

/** The mean and the median of a list of long values. */
public final class MeanMedian {
    private MeanMedian() {}

    /** Returns {mean, median} of a non-empty array. The array does not change. */
    public static double[] meanAndMedian(long[] items) {
        // A double sum does not overflow; the long sum of average() can.
        double mean = LongStream.of(items).asDoubleStream().average().orElseThrow();
        long[] s = items.clone();
        Arrays.sort(s);
        int mid = s.length / 2;
        double median = s.length % 2 == 1 ? s[mid] : ((double) s[mid - 1] + s[mid]) / 2;
        return new double[] {mean, median};
    }
}
