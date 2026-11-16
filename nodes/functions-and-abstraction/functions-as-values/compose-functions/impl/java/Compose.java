// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compose-functions/.

import java.util.List;
import java.util.Map;
import java.util.function.Function;

/** A generic compose helper. */
public final class Compose {
    private Compose() {}

    private static final Map<String, Function<Integer, Integer>> OPS = Map.of(
            "inc", x -> x + 1,
            "dec", x -> x - 1,
            "double", x -> 2 * x,
            "negate", x -> -x);

    /** Returns the composition of the functions. The last one runs first. */
    public static <T> Function<T, T> compose(List<Function<T, T>> fs) {
        return fs.stream().reduce(Function.identity(), (f, g) -> f.compose(g));
    }

    /** Composes the functions that ops names and applies the result to x. */
    public static int composeAll(List<String> ops, int x) {
        return compose(ops.stream().map(OPS::get).toList()).apply(x);
    }
}
