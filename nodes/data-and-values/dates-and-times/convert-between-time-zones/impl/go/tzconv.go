// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-between-time-zones/.

// Package tzconv converts a local date and time from one time zone to another.
package tzconv

import "time"

// offsetAt returns the UTC offset of loc, in seconds, at the instant t.
func offsetAt(loc *time.Location, t int64) int64 {
	_, offset := time.Unix(t, 0).In(loc).Zone()
	return int64(offset)
}

// LocalToInstant returns the instant of a local time in loc. A time in a gap
// moves forward by the length of the gap, and a time in an overlap gives the
// earlier of its two instants. time.Date does not promise either choice.
func LocalToInstant(year, month, day, hour, minute, second int, loc *time.Location) time.Time {
	t := time.Date(year, time.Month(month), day, hour, minute, second, 0, time.UTC).Unix()
	before := offsetAt(loc, t-86400)
	after := offsetAt(loc, t+86400)
	found := false
	var instant int64
	for _, offset := range []int64{before, after} {
		u := t - offset
		if offsetAt(loc, u) == offset && (!found || u < instant) {
			instant, found = u, true
		}
	}
	if !found {
		instant = t - before
	}
	return time.Unix(instant, 0)
}

// ConvertTimeZone returns the local time in toZone for a local time in
// fromZone, as YYYY-MM-DDTHH:MM:SS+HH:MM.
func ConvertTimeZone(year, month, day, hour, minute, second int, fromZone, toZone string) (string, error) {
	from, err := time.LoadLocation(fromZone)
	if err != nil {
		return "", err
	}
	to, err := time.LoadLocation(toZone)
	if err != nil {
		return "", err
	}
	instant := LocalToInstant(year, month, day, hour, minute, second, from)
	return instant.In(to).Format("2006-01-02T15:04:05-07:00"), nil
}
