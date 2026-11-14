// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-unix-timestamp/.

import java.time.Instant;
import java.time.OffsetDateTime;
import java.time.ZoneOffset;

public final class UnixTime {
    private UnixTime() {}

    /** Returns the seconds since 1970-01-01T00:00:00Z of a local time with the given UTC offset. */
    public static long unixTimestamp(
            int year, int month, int day, int hour, int minute, int second, int offsetMinutes) {
        ZoneOffset offset = ZoneOffset.ofTotalSeconds(offsetMinutes * 60);
        return OffsetDateTime.of(year, month, day, hour, minute, second, 0, offset).toEpochSecond();
    }

    /** Returns the current Unix time in whole seconds, rounded toward the past. */
    public static long nowUnixSeconds() {
        return Instant.now().getEpochSecond();
    }

    /** Returns the current Unix time in milliseconds. */
    public static long nowUnixMillis() {
        return System.currentTimeMillis();
    }
}
