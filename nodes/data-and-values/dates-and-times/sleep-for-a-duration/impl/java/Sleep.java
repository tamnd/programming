// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sleep-for-a-duration/.

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.TimeUnit;

/** Sleeps for a duration on a monotonic clock. */
public final class Sleep {
    private Sleep() {}

    /** A monotonic clock in nanoseconds. */
    @FunctionalInterface
    public interface Clock {
        long nanos();
    }

    /** A sleep for a number of nanoseconds. */
    @FunctionalInterface
    public interface Sleeper {
        void sleep(long nanos) throws InterruptedException;
    }

    /**
     * Sleeps until the clock reaches the deadline. A sleep that ends early is followed by a sleep
     * for the rest of the time. The loop compares the difference with zero, as the documentation
     * of System.nanoTime advises, because of the possibility of numerical overflow.
     */
    public static void sleepLoop(long durationNs, Clock clock, Sleeper sleeper) throws InterruptedException {
        long now = clock.nanos();
        long deadline = now + durationNs;
        while (deadline - now > 0) {
            sleeper.sleep(deadline - now);
            now = clock.nanos();
        }
    }

    /** Blocks the thread for at least durationNs nanoseconds. */
    public static void sleepFor(long durationNs) throws InterruptedException {
        sleepLoop(durationNs, System::nanoTime, TimeUnit.NANOSECONDS::sleep);
    }

    /** Runs sleepLoop on a fake clock and returns the durations it asks for. */
    public static long[] sleepRequests(long durationNs, long startNs, long[] readings) {
        long[] times = new long[readings.length + 1];
        times[0] = startNs;
        System.arraycopy(readings, 0, times, 1, readings.length);
        int[] next = {0};
        List<Long> requests = new ArrayList<>();
        try {
            sleepLoop(durationNs, () -> times[next[0]++], requests::add);
        } catch (InterruptedException e) {
            throw new IllegalStateException("the fake sleep does not throw", e);
        }
        return requests.stream().mapToLong(Long::longValue).toArray();
    }
}
