// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sum-and-multiply-a-list/.

import java.util.stream.LongStream;

/** The sum and the product of a list of long values. */
public final class SumProd {
    private SumProd() {}

    /** Returns {sum, product}. multiplyExact and addExact throw on overflow. */
    public static long[] sumAndProduct(long[] items) {
        long sum = LongStream.of(items).reduce(0L, Math::addExact);
        long product = LongStream.of(items).reduce(1L, Math::multiplyExact);
        return new long[] {sum, product};
    }
}
