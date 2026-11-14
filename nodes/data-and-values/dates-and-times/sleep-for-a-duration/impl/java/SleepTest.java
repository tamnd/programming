// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sleep-for-a-duration/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class SleepTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("one-call", 1000000L, 0L, new long[] {1000000L}, new long[] {1000000L}),
                Arguments.of("woke-late", 1000000L, 0L, new long[] {1080000L}, new long[] {1000000L}),
                Arguments.of("woke-early", 1000000L, 0L, new long[] {600000L, 1000000L}, new long[] {1000000L, 400000L}),
                Arguments.of("one-second", 1000000000L, 5000000000L, new long[] {6000050000L}, new long[] {1000000000L}),
                Arguments.of("zero", 0L, 0L, new long[] {}, new long[] {}),
                Arguments.of("negative", -5000000L, 0L, new long[] {}, new long[] {}),
                Arguments.of("most-negative", -4611686018427387904L, 0L, new long[] {}, new long[] {}),
                Arguments.of("several-early-wakes", 10000000L, 0L, new long[] {2000000L, 5000000L, 9999999L, 10000000L}, new long[] {10000000L, 8000000L, 5000000L, 1L}),
                Arguments.of("no-progress", 1000000L, 0L, new long[] {0L, 1000000L}, new long[] {1000000L, 1000000L}),
                Arguments.of("exact-deadline", 3000000L, 100L, new long[] {3000100L}, new long[] {3000000L}),
                Arguments.of("one-ns-short", 3000000L, 0L, new long[] {2999999L, 3000000L}, new long[] {3000000L, 1L}),
                Arguments.of("extra-readings", 1000000L, 0L, new long[] {1000000L, 5000000L}, new long[] {1000000L}),
                Arguments.of("negative-origin", 2000000000L, -5000000000L, new long[] {-4000000000L, -3000000000L}, new long[] {2000000000L, 1000000000L}),
                Arguments.of("origin-after-100-days", 1500000L, 8640000000000000L, new long[] {8640000001000000L, 8640000001500000L}, new long[] {1500000L, 500000L}),
                Arguments.of("one-nanosecond", 1L, 0L, new long[] {1000000L}, new long[] {1L}),
                Arguments.of("sub-millisecond", 1500000L, 0L, new long[] {1000000L, 1500000L}, new long[] {1500000L, 500000L}),
                Arguments.of("one-hour", 3600000000000L, 0L, new long[] {3600000000000L}, new long[] {3600000000000L}),
                Arguments.of("thirty-days", 2592000000000000L, 0L, new long[] {2147483647000000L, 2592000000000000L}, new long[] {2592000000000000L, 444516353000000L}));
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void sleepRequests(String name, long durationNs, long startNs, long[] readings, long[] want) {
        assertArrayEquals(want, Sleep.sleepRequests(durationNs, startNs, readings), name);
    }

    @Test
    void sleepForWaitsAtLeastTheDuration() throws InterruptedException {
        long start = System.nanoTime();
        Sleep.sleepFor(20_000_000L);
        assertTrue(System.nanoTime() - start >= 20_000_000L);
    }

    @Test
    void sleepForReturnsAtOnce() throws InterruptedException {
        for (long ns : new long[] {0L, -5_000_000L}) {
            long start = System.nanoTime();
            Sleep.sleepFor(ns);
            assertTrue(System.nanoTime() - start < 1_000_000_000L);
        }
    }
}
