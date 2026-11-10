// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-and-replace-with-a-regular-expression/.

import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/** Finds dates with a regular expression and writes them in another order. */
public final class ReformatDates {
    private ReformatDates() {}

    /** Returns the pattern of a date YYYY(sep)MM(sep)DD with no digit next to it. */
    static Pattern datePattern(String sep) {
        // Pattern.quote makes each special character of sep match only itself.
        String s = Pattern.quote(sep);
        return Pattern.compile("(?<![0-9])(?<year>[0-9]{4})" + s + "(?<month>[0-9]{2})" + s
                + "(?<day>[0-9]{2})(?![0-9])");
    }

    /** Changes each date YYYY(fromSep)MM(fromSep)DD in text to DD(toSep)MM(toSep)YYYY. */
    public static String reformatDates(String text, String fromSep, String toSep) {
        // In a replacement string, $ starts a group reference and \ escapes the next character.
        String sep = Matcher.quoteReplacement(toSep);
        return datePattern(fromSep).matcher(text).replaceAll("${day}" + sep + "${month}" + sep + "${year}");
    }

    /** Returns [year, month, day] for each date YYYY(sep)MM(sep)DD in text. */
    public static List<List<String>> findDates(String text, String sep) {
        return datePattern(sep).matcher(text).results()
                .map(m -> List.of(m.group("year"), m.group("month"), m.group("day")))
                .toList();
    }
}
