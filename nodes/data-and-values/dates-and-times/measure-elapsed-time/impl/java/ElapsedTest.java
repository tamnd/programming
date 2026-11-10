// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/measure-elapsed-time/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.ArrayList;
import java.util.List;
import java.util.function.LongSupplier;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ElapsedTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("one-and-a-half-ms", 1000000000L, 1001500000L, "1.500 ms"),
                Arguments.of("seconds", 0L, 2345678901L, "2345.678 ms"),
                Arguments.of("under-one-ms", 50000L, 750000L, "0.700 ms"),
                Arguments.of("zero", 123456789L, 123456789L, "0.000 ms"),
                Arguments.of("under-one-microsecond", 0L, 999L, "0.000 ms"),
                Arguments.of("truncate-not-round", 0L, 1999999L, "1.999 ms"),
                Arguments.of("leading-zero-in-fraction", 0L, 1005000L, "1.005 ms"),
                Arguments.of("exact-ms", 0L, 7000000L, "7.000 ms"),
                Arguments.of("one-hour", 0L, 3600000000000L, "3600000.000 ms"),
                Arguments.of("negative-readings", -5000000L, -3500000L, "1.500 ms"),
                Arguments.of("across-zero", -1000000L, 500000L, "1.500 ms"),
                Arguments.of("beyond-2-pow-53", 9007199254740993L, 9007199255741992L, "1.000 ms"),
                Arguments.of("near-int64-max", 9223372035854775807L, 9223372036854775807L, "1000.000 ms"),
                Arguments.of("near-int64-min", Long.MIN_VALUE, -9223372035854775808L, "1000.000 ms")
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void elapsedMs(String name, long startNs, long endNs, String want) {
        assertEquals(want, Elapsed.elapsedMs(startNs, endNs));
    }

    @Test
    void measureWithAFakeClock() {
        List<String> calls = new ArrayList<>();
        long[] readings = {5_000_000L, 6_500_000L};
        int[] next = {0};
        LongSupplier clock = () -> {
            calls.add("clock");
            return readings[next[0]++];
        };
        assertEquals(1_500_000L, Elapsed.measure(() -> calls.add("work"), clock));
        assertEquals(List.of("clock", "work", "clock"), calls);
    }

    @Test
    void measureWithTheMonotonicClock() {
        List<String> calls = new ArrayList<>();
        assertTrue(Elapsed.measure(() -> calls.add("work")) >= 0);
        assertEquals(List.of("work"), calls);
    }
}
