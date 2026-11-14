// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-between-time-zones/.

//! Converts a local date and time from one time zone to another. The standard
//! library has no time zones, so this module reads the TZif files of the system
//! tz database (RFC 9636), including the TZ string footer that gives the rules
//! after the last transition in the file.

use std::fs;

/// Days from 1970-01-01 to a date of the proleptic Gregorian calendar.
fn days_from_civil(y: i64, m: u32, d: u32) -> i64 {
    let y = if m <= 2 { y - 1 } else { y };
    let era = y.div_euclid(400);
    let yoe = y - era * 400;
    let mp = i64::from((m + 9) % 12);
    let doy = (153 * mp + 2) / 5 + i64::from(d) - 1;
    let doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    era * 146_097 + doe - 719_468
}

/// The date of a day number from days_from_civil.
fn civil_from_days(z: i64) -> (i64, u32, u32) {
    let z = z + 719_468;
    let era = z.div_euclid(146_097);
    let doe = z - era * 146_097;
    let yoe = (doe - doe / 1460 + doe / 36_524 - doe / 146_096) / 365;
    let doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    let mp = (5 * doy + 2) / 153;
    let d = (doy - (153 * mp + 2) / 5 + 1) as u32;
    let m = if mp < 10 { mp + 3 } else { mp - 9 } as u32;
    (yoe + era * 400 + i64::from(m <= 2), m, d)
}

fn is_leap(y: i64) -> bool {
    y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)
}

/// A day in a POSIX TZ string rule.
#[derive(Debug, Clone, Copy)]
enum Day {
    /// Jn: day 1 to 365, where February 29 is never counted.
    Julian(i64),
    /// n: day 0 to 365, where February 29 is counted.
    Zero(i64),
    /// Mm.w.d: day d (0 is Sunday) of week w (5 is the last) of month m.
    MonthWeek(u32, i64, i64),
}

impl Day {
    fn days(self, y: i64) -> i64 {
        match self {
            Day::Julian(n) => days_from_civil(y, 1, 1) + n - 1 + i64::from(is_leap(y) && n >= 60),
            Day::Zero(n) => days_from_civil(y, 1, 1) + n,
            Day::MonthWeek(m, w, d) => {
                let first = days_from_civil(y, m, 1);
                let next = if m == 12 {
                    days_from_civil(y + 1, 1, 1)
                } else {
                    days_from_civil(y, m + 1, 1)
                };
                let weekday = (first + 4).rem_euclid(7);
                let mut day = first + (d - weekday).rem_euclid(7) + 7 * (w - 1);
                if day >= next {
                    day -= 7;
                }
                day
            }
        }
    }
}

/// The rule of a TZ string: a fixed offset, or standard and daylight saving
/// time with the local times of the two transitions in each year.
#[derive(Debug, Clone, Copy)]
enum Rule {
    Fixed(i64),
    Dst {
        std: i64,
        dst: i64,
        start: (Day, i64),
        end: (Day, i64),
    },
}

impl Rule {
    fn offset_at(self, t: i64) -> i64 {
        match self {
            Rule::Fixed(offset) => offset,
            Rule::Dst {
                std,
                dst,
                start,
                end,
            } => {
                let (year, _, _) = civil_from_days((t + std).div_euclid(86_400));
                let begin = start.0.days(year) * 86_400 + start.1 - std;
                let finish = end.0.days(year) * 86_400 + end.1 - dst;
                let in_dst = if begin < finish {
                    begin <= t && t < finish
                } else {
                    !(finish <= t && t < begin)
                };
                if in_dst {
                    dst
                } else {
                    std
                }
            }
        }
    }
}

struct TzString<'a> {
    s: &'a [u8],
    i: usize,
}

impl TzString<'_> {
    fn peek(&self) -> Option<u8> {
        self.s.get(self.i).copied()
    }

    fn eat(&mut self, c: u8) -> bool {
        if self.peek() == Some(c) {
            self.i += 1;
            true
        } else {
            false
        }
    }

    fn name(&mut self) -> Result<(), String> {
        let begin = self.i;
        if self.eat(b'<') {
            while self.peek().is_some_and(|c| c != b'>') {
                self.i += 1;
            }
            if !self.eat(b'>') {
                return Err("unterminated zone abbreviation".into());
            }
        } else {
            while self.peek().is_some_and(|c| c.is_ascii_alphabetic()) {
                self.i += 1;
            }
        }
        if self.i - begin < 3 {
            Err("short zone abbreviation".into())
        } else {
            Ok(())
        }
    }

    fn number(&mut self) -> Result<i64, String> {
        let begin = self.i;
        while self.peek().is_some_and(|c| c.is_ascii_digit()) {
            self.i += 1;
        }
        std::str::from_utf8(&self.s[begin..self.i])
            .ok()
            .and_then(|n| n.parse().ok())
            .ok_or_else(|| "expected a number".into())
    }

    /// [+-]hh[:mm[:ss]] in seconds.
    fn time(&mut self) -> Result<i64, String> {
        let sign = if self.eat(b'-') {
            -1
        } else {
            self.eat(b'+');
            1
        };
        let mut seconds = self.number()? * 3600;
        if self.eat(b':') {
            seconds += self.number()? * 60;
            if self.eat(b':') {
                seconds += self.number()?;
            }
        }
        Ok(sign * seconds)
    }

    fn day(&mut self) -> Result<(Day, i64), String> {
        let day = if self.eat(b'J') {
            Day::Julian(self.number()?)
        } else if self.eat(b'M') {
            let m = self.number()?;
            if !self.eat(b'.') {
                return Err("expected .".into());
            }
            let w = self.number()?;
            if !self.eat(b'.') {
                return Err("expected .".into());
            }
            Day::MonthWeek(
                u32::try_from(m).map_err(|e| e.to_string())?,
                w,
                self.number()?,
            )
        } else {
            Day::Zero(self.number()?)
        };
        let time = if self.eat(b'/') { self.time()? } else { 7200 };
        Ok((day, time))
    }

    /// The offsets in a TZ string are positive west of Greenwich, so they
    /// are negated to get UTC offsets.
    fn rule(&mut self) -> Result<Rule, String> {
        self.name()?;
        let std = -self.time()?;
        if self.peek().is_none() {
            return Ok(Rule::Fixed(std));
        }
        self.name()?;
        let dst = match self.peek() {
            Some(c) if c == b'+' || c == b'-' || c.is_ascii_digit() => -self.time()?,
            _ => std + 3600,
        };
        if !self.eat(b',') {
            return Err("TZ string without transition rules".into());
        }
        let start = self.day()?;
        if !self.eat(b',') {
            return Err("expected ,".into());
        }
        let end = self.day()?;
        Ok(Rule::Dst {
            std,
            dst,
            start,
            end,
        })
    }
}

/// A time zone read from a TZif file.
#[derive(Debug)]
pub struct Zone {
    transitions: Vec<i64>,
    offsets: Vec<i64>,
    first: i64,
    rule: Option<Rule>,
}

fn be(b: &[u8]) -> i64 {
    b.iter().fold(0, |n, &x| (n << 8) | i64::from(x))
}

impl Zone {
    /// Reads the zone from the tz database in TZDIR or /usr/share/zoneinfo.
    pub fn load(name: &str) -> Result<Zone, String> {
        if name.is_empty() || name.starts_with('/') || name.split('/').any(|p| p == "..") {
            return Err(format!("bad zone name {name}"));
        }
        let dir = std::env::var("TZDIR").unwrap_or_else(|_| "/usr/share/zoneinfo".into());
        let data = fs::read(format!("{dir}/{name}")).map_err(|e| format!("{name}: {e}"))?;
        Zone::parse(&data)
    }

    /// Parses a TZif file. For version 2 and later, the version 1 data block
    /// is skipped and the 64-bit data block and the footer are used.
    pub fn parse(data: &[u8]) -> Result<Zone, String> {
        let bad = || "not a TZif file".to_string();
        let header = |at: usize| -> Result<[usize; 6], String> {
            let h = data.get(at..at + 44).ok_or_else(bad)?;
            if &h[..4] != b"TZif" {
                return Err(bad());
            }
            let mut counts = [0; 6];
            for (k, c) in counts.iter_mut().enumerate() {
                *c = be(&h[20 + 4 * k..24 + 4 * k]) as usize;
            }
            Ok(counts)
        };
        let block_len = |c: [usize; 6], size: usize| {
            let [isut, isstd, leap, time, typ, chars] = c;
            time * size + time + typ * 6 + chars + leap * (size + 4) + isstd + isut
        };
        let mut at = 0;
        let mut counts = header(at)?;
        let mut size = 4;
        if data[4] >= b'2' {
            at += 44 + block_len(counts, 4);
            counts = header(at)?;
            size = 8;
        }
        at += 44;
        let [_, _, _, timecnt, typecnt, _] = counts;
        let end = at + block_len(counts, size);
        let block = data.get(at..end).ok_or_else(bad)?;
        let signed = |b: &[u8]| {
            let n = be(b);
            if size == 4 {
                i64::from(n as u32 as i32)
            } else {
                n
            }
        };
        let transitions: Vec<i64> = (0..timecnt)
            .map(|i| signed(&block[i * size..(i + 1) * size]))
            .collect();
        let index = &block[timecnt * size..timecnt * (size + 1)];
        let types = &block[timecnt * (size + 1)..timecnt * (size + 1) + typecnt * 6];
        let utoff = |k: usize| i64::from(be(&types[k * 6..k * 6 + 4]) as u32 as i32);
        if typecnt == 0 {
            return Err(bad());
        }
        let offsets = index.iter().map(|&k| utoff(usize::from(k))).collect();
        let mut rule = None;
        if size == 8 && data.get(end) == Some(&b'\n') {
            let rest = &data[end + 1..];
            let footer = &rest[..rest.iter().position(|&c| c == b'\n').unwrap_or(rest.len())];
            if !footer.is_empty() {
                rule = Some(TzString { s: footer, i: 0 }.rule()?);
            }
        }
        Ok(Zone {
            transitions,
            offsets,
            first: utoff(0),
            rule,
        })
    }

    /// The UTC offset, in seconds, at t seconds after the epoch.
    pub fn offset_at(&self, t: i64) -> i64 {
        match self.transitions.last() {
            Some(&last) if t >= last => self
                .rule
                .map_or(self.offsets[self.offsets.len() - 1], |r| r.offset_at(t)),
            None => self.rule.map_or(self.first, |r| r.offset_at(t)),
            _ => match self.transitions.partition_point(|&x| x <= t) {
                0 => self.first,
                n => self.offsets[n - 1],
            },
        }
    }

    /// The instant of a local time, given as seconds after the epoch as if
    /// it were UTC. A time in a gap moves forward by the length of the gap,
    /// and a time in an overlap gives the earlier of its two instants.
    pub fn local_to_instant(&self, local: i64) -> i64 {
        let before = self.offset_at(local - 86_400);
        let after = self.offset_at(local + 86_400);
        [before, after]
            .into_iter()
            .map(|offset| local - offset)
            .filter(|&u| local - u == self.offset_at(u))
            .min()
            .unwrap_or(local - before)
    }
}

/// Returns the local time in to_zone for a local time in from_zone, as
/// YYYY-MM-DDTHH:MM:SS+HH:MM.
#[allow(clippy::too_many_arguments)]
pub fn convert_time_zone(
    year: i64,
    month: u32,
    day: u32,
    hour: u32,
    minute: u32,
    second: u32,
    from_zone: &str,
    to_zone: &str,
) -> Result<String, String> {
    let from = Zone::load(from_zone)?;
    let to = Zone::load(to_zone)?;
    let local = days_from_civil(year, month, day) * 86_400
        + i64::from(hour) * 3600
        + i64::from(minute) * 60
        + i64::from(second);
    let instant = from.local_to_instant(local);
    let offset = to.offset_at(instant);
    let there = instant + offset;
    let (y, m, d) = civil_from_days(there.div_euclid(86_400));
    let s = there.rem_euclid(86_400);
    let sign = if offset < 0 { '-' } else { '+' };
    let a = offset.abs();
    Ok(format!(
        "{y:04}-{m:02}-{d:02}T{:02}:{:02}:{:02}{sign}{:02}:{:02}",
        s / 3600,
        s % 3600 / 60,
        s % 60,
        a / 3600,
        a % 3600 / 60
    ))
}
