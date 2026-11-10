// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-and-format-a-date/.

import java.time.OffsetDateTime;
import java.time.ZoneOffset;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;
import java.util.Locale;
import java.util.regex.Pattern;

final class ToUtc {
  // The date-time of RFC 3339 section 5.6, with a fraction of 1 to 3 digits.
  // ISO_OFFSET_DATE_TIME accepts more than this (no seconds, the offset +07),
  // so the pattern checks the syntax first.
  private static final Pattern RFC3339 =
      Pattern.compile(
          "[0-9]{4}-[0-9]{2}-[0-9]{2}[Tt]([01][0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9]"
              + "(\\.[0-9]{1,3})?([Zz]|[+-]([01][0-9]|2[0-3]):[0-5][0-9])");

  // "uuuu" is the year. "YYYY" is the week-based year, which is a different
  // number in the last days of some years.
  private static final DateTimeFormatter OUT =
      DateTimeFormatter.ofPattern("uuuu-MM-dd'T'HH:mm:ss.SSS'Z'", Locale.ROOT);

  private ToUtc() {}

  /**
   * Returns the instant of an RFC 3339 timestamp in UTC, as YYYY-MM-DDTHH:MM:SS.sssZ.
   *
   * @throws IllegalArgumentException when timestamp is not an RFC 3339 date-time with an offset,
   *     or when its date is not in the calendar
   */
  static String toUtc(String timestamp) {
    if (!RFC3339.matcher(timestamp).matches()) {
      throw new IllegalArgumentException("not an RFC 3339 date-time: \"" + timestamp + "\"");
    }
    try {
      // ISO_OFFSET_DATE_TIME uses ResolverStyle.STRICT, so 2026-02-29 is an error.
      OffsetDateTime value =
          OffsetDateTime.parse(
              timestamp.toUpperCase(Locale.ROOT), DateTimeFormatter.ISO_OFFSET_DATE_TIME);
      return value.withOffsetSameInstant(ZoneOffset.UTC).format(OUT);
    } catch (DateTimeParseException e) {
      throw new IllegalArgumentException("not a date in the calendar: \"" + timestamp + "\"", e);
    }
  }
}
