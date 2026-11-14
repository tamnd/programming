// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-current-date-and-time/.

import java.time.Clock;
import java.time.Instant;
import java.time.ZoneOffset;
import java.time.format.DateTimeFormatter;
import java.time.temporal.ChronoUnit;
import java.util.Locale;

/** Reads the current date and time and writes it in UTC. */
public final class Now {
    private Now() {}

    // Instant.toString drops a zero fraction, so the pattern fixes three digits of milliseconds.
    private static final DateTimeFormatter UTC_MILLIS =
            DateTimeFormatter.ofPattern("uuuu-MM-dd'T'HH:mm:ss.SSS'Z'", Locale.ROOT).withZone(ZoneOffset.UTC);

    /** Returns the UTC date and time of epochNs as "YYYY-MM-DDTHH:MM:SS.mmmZ". */
    public static String utcTimestamp(long epochNs) {
        // floorDiv and floorMod go toward the past, so the nanoseconds of an Instant are never negative.
        Instant instant = Instant.ofEpochSecond(Math.floorDiv(epochNs, 1_000_000_000L), Math.floorMod(epochNs, 1_000_000_000L));
        return UTC_MILLIS.format(instant.truncatedTo(ChronoUnit.MILLIS));
    }

    /** Reads the clock once and returns the UTC date and time of the reading. */
    public static String nowUtc(Clock clock) {
        Instant now = clock.instant();
        return utcTimestamp(Math.addExact(Math.multiplyExact(now.getEpochSecond(), 1_000_000_000L), now.getNano()));
    }

    /** Reads the system clock, which is the best available clock of the system in UTC. */
    public static String nowUtc() {
        return nowUtc(Clock.systemUTC());
    }
}
