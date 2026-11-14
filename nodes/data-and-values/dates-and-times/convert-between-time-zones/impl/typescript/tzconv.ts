// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-between-time-zones/.

const formatters = new Map<string, Intl.DateTimeFormat>();

function formatter(zone: string): Intl.DateTimeFormat {
  let f = formatters.get(zone);
  if (f === undefined) {
    // The constructor throws a RangeError for an unknown zone name.
    f = new Intl.DateTimeFormat("en-US", {
      timeZone: zone,
      hourCycle: "h23",
      year: "numeric",
      month: "numeric",
      day: "numeric",
      hour: "numeric",
      minute: "numeric",
      second: "numeric",
    });
    formatters.set(zone, f);
  }
  return f;
}

// Returns the UTC offset of zone, in seconds, at t seconds after the epoch.
export function offsetSeconds(zone: string, t: number): number {
  const p = { year: 0, month: 1, day: 1, hour: 0, minute: 0, second: 0 };
  for (const part of formatter(zone).formatToParts(new Date(t * 1000))) {
    if (part.type in p) {
      p[part.type as keyof typeof p] = Number(part.value);
    }
  }
  const local = Date.UTC(p.year, p.month - 1, p.day, p.hour, p.minute, p.second) / 1000;
  return local - t;
}

// Returns the instant, in seconds after the epoch, of a local time in zone.
// A time in a gap moves forward by the length of the gap, and a time in an
// overlap gives the earlier of its two instants.
export function localToInstant(
  year: number,
  month: number,
  day: number,
  hour: number,
  minute: number,
  second: number,
  zone: string,
): number {
  const t = Date.UTC(year, month - 1, day, hour, minute, second) / 1000;
  const before = offsetSeconds(zone, t - 86400);
  const after = offsetSeconds(zone, t + 86400);
  let instant: number | null = null;
  for (const offset of [before, after]) {
    const u = t - offset;
    if (offsetSeconds(zone, u) === offset && (instant === null || u < instant)) {
      instant = u;
    }
  }
  return instant === null ? t - before : instant;
}

function formatOffset(offset: number): string {
  const sign = offset < 0 ? "-" : "+";
  const abs = Math.abs(offset);
  const hh = String(Math.floor(abs / 3600)).padStart(2, "0");
  const mm = String(Math.floor((abs % 3600) / 60)).padStart(2, "0");
  return `${sign}${hh}:${mm}`;
}

export function convertTimeZone(
  year: number,
  month: number,
  day: number,
  hour: number,
  minute: number,
  second: number,
  fromZone: string,
  toZone: string,
): string {
  const instant = localToInstant(year, month, day, hour, minute, second, fromZone);
  const offset = offsetSeconds(toZone, instant);
  const local = new Date((instant + offset) * 1000).toISOString().slice(0, 19);
  return local + formatOffset(offset);
}
