// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-duration/.

import java.time.Duration;

/** Formats a duration in seconds as HH:MM:SS. */
public final class DurationFormat {
    private DurationFormat() {}

    /** Returns the duration as HH:MM:SS. The hours do not wrap at 24. */
    public static String formatDuration(long seconds) {
        Duration d = Duration.ofSeconds(seconds).abs();
        String sign = seconds < 0 ? "-" : "";
        return String.format("%s%02d:%02d:%02d", sign, d.toHours(), d.toMinutesPart(), d.toSecondsPart());
    }
}
