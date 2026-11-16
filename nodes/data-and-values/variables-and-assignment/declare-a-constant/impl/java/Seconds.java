// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-constant/.

/** Named constants for a count of seconds. */
public final class Seconds {
    private Seconds() {}

    // static final fields of a primitive type, initialized with constant
    // expressions, are constant variables: the compiler knows their values.
    static final long SECONDS_PER_MINUTE = 60;
    static final long MINUTES_PER_HOUR = 60;
    static final long HOURS_PER_DAY = 24;
    static final long SECONDS_PER_DAY = SECONDS_PER_MINUTE * MINUTES_PER_HOUR * HOURS_PER_DAY;

    /** Returns the number of seconds in days days of 24 hours. */
    public static long secondsIn(long days) {
        return days * SECONDS_PER_DAY;
    }

    public static void main(String[] args) {
        System.out.println(secondsIn(7));
    }
}
