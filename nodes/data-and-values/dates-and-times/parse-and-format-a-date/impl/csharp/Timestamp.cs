// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-and-format-a-date/.

using System.Globalization;
using System.Text.RegularExpressions;

public static partial class Timestamp
{
    // The date-time of RFC 3339 section 5.6, with a fraction of 1 to 3 digits.
    // The format "zzz" also accepts +0700, so the pattern checks the syntax
    // first. [0-9] is ASCII only; \d also matches other digits. \z is the end
    // of the text; $ also matches before a final line end.
    [GeneratedRegex(@"^[0-9]{4}-[0-9]{2}-[0-9]{2}[Tt]([01][0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9](\.[0-9]{1,3})?([Zz]|[+-]([01][0-9]|2[0-3]):[0-5][0-9])\z")]
    private static partial Regex Rfc3339();

    // "FFF" is 0 to 3 fraction digits. "MM" is the month and "mm" is the minute.
    private static readonly string[] Formats =
    [
        "yyyy-MM-dd'T'HH:mm:ss.FFFzzz",
        "yyyy-MM-dd'T'HH:mm:ss.FFF'Z'",
    ];

    /// <summary>
    /// Returns the instant of an RFC 3339 timestamp in UTC, as YYYY-MM-DDTHH:MM:SS.sssZ.
    /// </summary>
    /// <exception cref="FormatException">
    /// timestamp is not an RFC 3339 date-time with an offset, or its date is not in the calendar.
    /// </exception>
    public static string ToUtc(string timestamp)
    {
        if (!Rfc3339().IsMatch(timestamp))
        {
            throw new FormatException($"not an RFC 3339 date-time: \"{timestamp}\"");
        }
        // The second format has no offset specifier. AssumeUniversal gives it
        // the offset +00:00; with no style, the parser uses the offset of the
        // local time zone.
        DateTimeOffset value = DateTimeOffset.ParseExact(
            timestamp.ToUpperInvariant(),
            Formats,
            CultureInfo.InvariantCulture,
            DateTimeStyles.AssumeUniversal);
        return value.UtcDateTime.ToString("yyyy-MM-dd'T'HH:mm:ss.fff'Z'", CultureInfo.InvariantCulture);
    }
}
