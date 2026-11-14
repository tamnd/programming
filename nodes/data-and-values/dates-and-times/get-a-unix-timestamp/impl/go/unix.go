// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-unix-timestamp/.

package unix

import "time"

// UnixTimestamp returns the seconds since 1970-01-01T00:00:00Z of a local
// time with the given UTC offset in minutes.
func UnixTimestamp(year, month, day, hour, minute, second, offsetMinutes int) int64 {
	zone := time.FixedZone("", offsetMinutes*60)
	return time.Date(year, time.Month(month), day, hour, minute, second, 0, zone).Unix()
}

// NowUnixSeconds returns the current Unix time in whole seconds.
func NowUnixSeconds() int64 {
	return time.Now().Unix()
}

// NowUnixMillis returns the current Unix time in whole milliseconds.
func NowUnixMillis() int64 {
	return time.Now().UnixMilli()
}
