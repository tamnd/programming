// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-higher-order-function/.

import java.util.function.Function;

/** A function that takes a function and returns a function. */
public final class HigherOrder {
    private HigherOrder() {}

    /** Returns a function that applies f to its argument times times. */
    public static Function<Integer, Integer> repeat(Function<Integer, Integer> f, int times) {
        return x -> {
            int value = x;
            for (int i = 0; i < times; i++) {
                value = f.apply(value);
            }
            return value;
        };
    }

    /** Returns n / 2 for an even n and 3n + 1 for an odd n. */
    public static int collatzStep(int n) {
        return n % 2 == 0 ? n / 2 : 3 * n + 1;
    }

    /** Returns the value after times Collatz steps from start. */
    public static int collatzAfter(int start, int times) {
        return repeat(HigherOrder::collatzStep, times).apply(start);
    }
}
