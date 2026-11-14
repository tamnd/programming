// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-duration/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.time.Duration;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class DurationFormatTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("zero", 0L, "00:00:00"),
                Arguments.of("seconds-only", 59L, "00:00:59"),
                Arguments.of("one-minute", 60L, "00:01:00"),
                Arguments.of("one-hour", 3600L, "01:00:00"),
                Arguments.of("hours-minutes-seconds", 3723L, "01:02:03"),
                Arguments.of("just-under-an-hour", 3599L, "00:59:59"),
                Arguments.of("just-under-a-day", 86399L, "23:59:59"),
                Arguments.of("one-day", 86400L, "24:00:00"),
                Arguments.of("more-than-a-day", 90061L, "25:01:01"),
                Arguments.of("hundred-hours", 360000L, "100:00:00"),
                Arguments.of("one-week", 604800L, "168:00:00"),
                Arguments.of("one-year", 31536000L, "8760:00:00"),
                Arguments.of("minus-one-second", -1L, "-00:00:01"),
                Arguments.of("negative-minute", -61L, "-00:01:01"),
                Arguments.of("negative-hours", -3723L, "-01:02:03"),
                Arguments.of("negative-day", -90061L, "-25:01:01"),
                Arguments.of("beyond-go-duration", 10000000000L, "2777777:46:40"),
                Arguments.of("largest", 1000000000000000L, "277777777777:46:40"),
                Arguments.of("most-negative", -1000000000000000L, "-277777777777:46:40"));
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void format(String name, long seconds, String want) {
        assertEquals(want, DurationFormat.formatDuration(seconds));
    }

    @Test
    void isoForm() {
        assertEquals("PT25H1M1S", Duration.ofSeconds(90061).toString());
        assertEquals("PT-1M-1S", Duration.ofSeconds(-61).toString());
    }
}
