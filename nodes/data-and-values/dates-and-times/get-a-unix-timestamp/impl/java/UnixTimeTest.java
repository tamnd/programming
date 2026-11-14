// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-unix-timestamp/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.time.Instant;
import java.time.OffsetDateTime;
import java.time.ZoneOffset;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class UnixTimeTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("epoch", 1970, 1, 1, 0, 0, 0, 0, 0L),
                Arguments.of("billennium", 2001, 9, 9, 1, 46, 40, 0, 1000000000L),
                Arguments.of("negative-offset", 2024, 3, 10, 2, 0, 0, -300, 1710054000L),
                Arguments.of("positive-offset", 2024, 3, 10, 16, 0, 0, 540, 1710054000L),
                Arguments.of("one-second-before-epoch", 1969, 12, 31, 23, 59, 59, 0, -1L),
                Arguments.of("offset-moves-before-epoch", 1970, 1, 1, 0, 0, 0, 60, -3600L),
                Arguments.of("offset-moves-to-next-year", 2023, 12, 31, 19, 0, 0, -300, 1704067200L),
                Arguments.of("offset-of-45-minutes", 2024, 1, 1, 5, 45, 0, 345, 1704067200L),
                Arguments.of("offset-of-minus-9-30", 2023, 12, 31, 14, 30, 0, -570, 1704067200L),
                Arguments.of("offset-plus-14", 1970, 1, 1, 14, 0, 0, 840, 0L),
                Arguments.of("offset-minus-12", 1969, 12, 31, 12, 0, 0, -720, 0L),
                Arguments.of("leap-day", 2024, 2, 29, 0, 0, 0, 0, 1709164800L),
                Arguments.of("year-2000-is-leap", 2000, 2, 29, 0, 0, 0, 0, 951782400L),
                Arguments.of("year-1900-is-not-leap", 1900, 3, 1, 0, 0, 0, 0, -2203891200L),
                Arguments.of("int32-max", 2038, 1, 19, 3, 14, 7, 0, 2147483647L),
                Arguments.of("after-int32-max", 2038, 1, 19, 3, 14, 8, 0, 2147483648L),
                Arguments.of("int32-min", 1901, 12, 13, 20, 45, 52, 0, -2147483648L),
                Arguments.of("year-99-is-not-1999", 99, 12, 31, 23, 59, 59, 0, -59011459201L),
                Arguments.of("first-second-of-year-1", 1, 1, 1, 0, 0, 0, 0, -62135596800L),
                Arguments.of("last-second-of-year-9999", 9999, 12, 31, 23, 59, 59, 0, 253402300799L)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void unixTimestamp(
            String name, int year, int month, int day, int hour, int minute, int second, int offset, long want) {
        assertEquals(want, UnixTime.unixTimestamp(year, month, day, hour, minute, second, offset));
        OffsetDateTime back = Instant.ofEpochSecond(want).atOffset(ZoneOffset.ofTotalSeconds(offset * 60));
        assertEquals(OffsetDateTime.of(year, month, day, hour, minute, second, 0, back.getOffset()), back);
    }

    @Test
    void integerDivisionOfMillisTruncatesTowardZero() {
        assertEquals(-1L, -1500L / 1000L);
        assertEquals(-2L, Math.floorDiv(-1500L, 1000L));
        assertEquals(-2L, Instant.ofEpochMilli(-1500L).getEpochSecond());
    }

    @Test
    void now() {
        long seconds = UnixTime.nowUnixSeconds();
        long millis = UnixTime.nowUnixMillis();
        assertTrue(seconds >= 1_704_067_200L);
        long diff = Math.floorDiv(millis, 1000L) - seconds;
        assertTrue(diff >= 0 && diff <= 1);
    }
}
