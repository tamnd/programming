// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/measure-elapsed-time/.

import java.util.Locale;
import java.util.function.LongSupplier;

/** Measures the time that a piece of work takes. */
public final class Elapsed {
    private Elapsed() {}

    /** Runs work once and returns the time it took, in nanoseconds of System.nanoTime. */
    public static long measure(Runnable work) {
        // nanoTime is not related to the wall clock. Only a difference of two values has a meaning.
        return measure(work, System::nanoTime);
    }

    /** Runs work once and returns the time it took, in nanoseconds of clock. */
    public static long measure(Runnable work, LongSupplier clock) {
        long start = clock.getAsLong();
        work.run();
        return clock.getAsLong() - start;
    }

    /** Returns endNs - startNs as "M.UUU ms", with the rest under one microsecond dropped. */
    public static String elapsedMs(long startNs, long endNs) {
        long ns = endNs - startNs;
        // Locale.ROOT keeps the digits ASCII. Division of a long that is not negative truncates.
        return String.format(Locale.ROOT, "%d.%03d ms", ns / 1_000_000, ns / 1_000 % 1_000);
    }
}
