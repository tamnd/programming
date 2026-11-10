// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-and-format-a-date/.

//! The Rust standard library has no calendar: std::time measures durations
//! and instants, but it has no years, months or days. So this crate parses the
//! text by hand and counts days with the algorithms of Howard Hinnant.

use std::error::Error;
use std::fmt;

/// The error of [`to_utc`]: the text is not an RFC 3339 date-time with an
/// offset, or its date is not in the calendar.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ParseError {
    input: String,
}

impl fmt::Display for ParseError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "not an RFC 3339 date-time: {:?}", self.input)
    }
}

impl Error for ParseError {}

/// Returns the instant of an RFC 3339 timestamp in UTC, as
/// YYYY-MM-DDTHH:MM:SS.sssZ. The fraction can have 1 to 3 digits.
pub fn to_utc(timestamp: &str) -> Result<String, ParseError> {
    let millis = parse(timestamp.as_bytes()).ok_or_else(|| ParseError {
        input: timestamp.to_string(),
    })?;
    let days = millis.div_euclid(86_400_000);
    let ms = millis.rem_euclid(86_400_000);
    let (year, month, day) = civil_from_days(days);
    Ok(format!(
        "{year:04}-{month:02}-{day:02}T{:02}:{:02}:{:02}.{:03}Z",
        ms / 3_600_000,
        ms / 60_000 % 60,
        ms / 1000 % 60,
        ms % 1000
    ))
}

/// Returns the milliseconds from 1970-01-01T00:00:00Z, or None when the text
/// is not valid.
fn parse(b: &[u8]) -> Option<i64> {
    let year = number(b, 0, 4)?;
    let month = number(b, 5, 2)?;
    let day = number(b, 8, 2)?;
    let hour = number(b, 11, 2)?;
    let minute = number(b, 14, 2)?;
    let second = number(b, 17, 2)?;
    if b[4] != b'-'
        || b[7] != b'-'
        || !matches!(b[10], b'T' | b't')
        || b[13] != b':'
        || b[16] != b':'
    {
        return None;
    }
    if !(1..=12).contains(&month) || day < 1 || day > days_in_month(year, month) {
        return None;
    }
    if hour > 23 || minute > 59 || second > 59 {
        return None;
    }
    // The fraction: a dot and 1 to 3 digits, padded to milliseconds.
    let mut i = 19;
    let mut ms = 0;
    if b.get(i) == Some(&b'.') {
        i += 1;
        let start = i;
        while i < b.len() && b[i].is_ascii_digit() && i - start < 3 {
            ms = ms * 10 + i64::from(b[i] - b'0');
            i += 1;
        }
        let digits = i - start;
        if digits == 0 {
            return None;
        }
        ms *= 10_i64.pow(3 - digits as u32);
    }
    // The offset: Z, or a sign, two digits of hours, a colon and two digits of
    // minutes. -00:00 is the same instant as Z (RFC 3339 section 4.3).
    let offset = match b.get(i)? {
        b'Z' | b'z' if i + 1 == b.len() => 0,
        &sign @ (b'+' | b'-') if i + 6 == b.len() && b[i + 3] == b':' => {
            let hours = number(b, i + 1, 2)?;
            let minutes = number(b, i + 4, 2)?;
            if hours > 23 || minutes > 59 {
                return None;
            }
            let seconds = hours * 3600 + minutes * 60;
            if sign == b'+' { seconds } else { -seconds }
        }
        _ => return None,
    };
    let seconds = days_from_civil(year, month, day) * 86_400 + hour * 3600 + minute * 60 + second;
    Some((seconds - offset) * 1000 + ms)
}

/// Returns the number made of the ASCII digits b[start..start + len], or None.
fn number(b: &[u8], start: usize, len: usize) -> Option<i64> {
    let digits = b.get(start..start + len)?;
    digits.iter().try_fold(0, |n, &c| {
        c.is_ascii_digit().then(|| n * 10 + i64::from(c - b'0'))
    })
}

fn days_in_month(year: i64, month: i64) -> i64 {
    let leap = year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    match month {
        2 if leap => 29,
        2 => 28,
        4 | 6 | 9 | 11 => 30,
        _ => 31,
    }
}

/// Returns the days from 1970-01-01 to the date. A year starts on 1 March
/// here, so the leap day is the last day of a year.
fn days_from_civil(year: i64, month: i64, day: i64) -> i64 {
    let y = if month <= 2 { year - 1 } else { year };
    let era = y.div_euclid(400);
    let yoe = y - era * 400;
    let doy = (153 * (if month > 2 { month - 3 } else { month + 9 }) + 2) / 5 + day - 1;
    let doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    era * 146_097 + doe - 719_468
}

/// Returns the date that is the given number of days after 1970-01-01.
fn civil_from_days(days: i64) -> (i64, i64, i64) {
    let z = days + 719_468;
    let era = z.div_euclid(146_097);
    let doe = z - era * 146_097;
    let yoe = (doe - doe / 1460 + doe / 36_524 - doe / 146_096) / 365;
    let doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    let mp = (5 * doy + 2) / 153;
    let day = doy - (153 * mp + 2) / 5 + 1;
    let month = if mp < 10 { mp + 3 } else { mp - 9 };
    let year = yoe + era * 400 + i64::from(month <= 2);
    (year, month, day)
}
