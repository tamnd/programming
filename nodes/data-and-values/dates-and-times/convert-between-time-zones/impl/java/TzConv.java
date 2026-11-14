// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-between-time-zones/.

import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.ZonedDateTime;
import java.time.format.DateTimeFormatter;

/** Converts a local date and time from one time zone to another. */
public final class TzConv {
    private static final DateTimeFormatter FORMAT =
            DateTimeFormatter.ofPattern("uuuu-MM-dd'T'HH:mm:ssxxx");

    private TzConv() {}

    /**
     * Returns the local time in toZone for a local time in fromZone. ZonedDateTime.of moves a time in a
     * gap forward by the length of the gap and uses the earlier offset in an overlap.
     */
    public static String convertTimeZone(
            int year, int month, int day, int hour, int minute, int second, String fromZone, String toZone) {
        LocalDateTime local = LocalDateTime.of(year, month, day, hour, minute, second);
        ZonedDateTime from = ZonedDateTime.of(local, ZoneId.of(fromZone));
        return from.withZoneSameInstant(ZoneId.of(toZone)).format(FORMAT);
    }
}
