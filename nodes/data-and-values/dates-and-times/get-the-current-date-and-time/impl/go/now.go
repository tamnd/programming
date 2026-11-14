// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-current-date-and-time/.

// Package now reads the current date and time and writes it in UTC.
package now

import "time"

// utcMillis is the layout of the result. Go writes the fraction .000 with
// three digits and drops the rest; it does not round.
const utcMillis = "2006-01-02T15:04:05.000Z07:00"

// UTCTimestamp returns the UTC date and time of epochNs as
// "YYYY-MM-DDTHH:MM:SS.mmmZ".
func UTCTimestamp(epochNs int64) string {
	// time.Unix keeps the nanoseconds in [0, 1e9), so a negative reading goes
	// toward the past. UTC makes the layout write Z for the offset.
	return time.Unix(0, epochNs).UTC().Format(utcMillis)
}

// NowUTC reads the clock once and returns the UTC date and time of the
// reading. time.Now is the real clock.
func NowUTC(clock func() time.Time) string {
	return UTCTimestamp(clock().UnixNano())
}
