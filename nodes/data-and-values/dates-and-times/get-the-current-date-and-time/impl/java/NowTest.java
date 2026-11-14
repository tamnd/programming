// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-current-date-and-time/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.time.Clock;
import java.time.Instant;
import java.time.ZoneId;
import java.time.ZoneOffset;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class NowTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("epoch", 0L, "1970-01-01T00:00:00.000Z"),
                Arguments.of("billennium", 1000000000000000000L, "2001-09-09T01:46:40.000Z"),
                Arguments.of("with-milliseconds", 1710054000123456789L, "2024-03-10T07:00:00.123Z"),
                Arguments.of("leap-day", 1709208000000000000L, "2024-02-29T12:00:00.000Z"),
                Arguments.of("truncate-not-round", 999999L, "1970-01-01T00:00:00.000Z"),
                Arguments.of("one-ns-before-epoch", -1L, "1969-12-31T23:59:59.999Z"),
                Arguments.of("one-ms-before-epoch", -1000000L, "1969-12-31T23:59:59.999Z"),
                Arguments.of("just-over-one-ms-before-epoch", -1000001L, "1969-12-31T23:59:59.998Z"),
                Arguments.of("last-ms-of-a-year", 1704067199999999999L, "2023-12-31T23:59:59.999Z"),
                Arguments.of("year-2000-is-leap", 951782400000000000L, "2000-02-29T00:00:00.000Z"),
                Arguments.of("year-2100-is-not-leap", 4107542399999999999L, "2100-02-28T23:59:59.999Z"),
                Arguments.of("after-int32-seconds", 2147483648000000000L, "2038-01-19T03:14:08.000Z"),
                Arguments.of("int64-max", 9223372036854775807L, "2262-04-11T23:47:16.854Z"),
                Arguments.of("int64-min", Long.MIN_VALUE, "1677-09-21T00:12:43.145Z")
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void utcTimestamp(String name, long epochNs, String want) {
        assertEquals(want, Now.utcTimestamp(epochNs));
    }

    @Test
    void instantToStringDropsAZeroFraction() {
        assertEquals("1970-01-01T00:00:00Z", Instant.EPOCH.toString());
        assertEquals("1970-01-01T00:00:00.000Z", Now.utcTimestamp(0L));
    }

    @Test
    void nowUtcWithAFixedClock() {
        Clock clock = Clock.fixed(Instant.ofEpochSecond(1_710_054_000L, 123_456_789L), ZoneId.of("America/New_York"));
        // The zone of the clock does not change the instant.
        assertEquals("2024-03-10T07:00:00.123Z", Now.nowUtc(clock));
    }

    @Test
    void nowUtcWithTheRealClock() {
        String got = Now.nowUtc();
        assertTrue(got.matches("\\d{4}-\\d\\d-\\d\\dT\\d\\d:\\d\\d:\\d\\d\\.\\d{3}Z"), got);
        assertTrue(Integer.parseInt(got.substring(0, 4)) >= 2024, got);
        assertEquals(ZoneOffset.UTC, Clock.systemUTC().getZone());
    }
}
