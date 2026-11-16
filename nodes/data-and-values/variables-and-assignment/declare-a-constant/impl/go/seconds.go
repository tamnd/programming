// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-constant/.

// Package seconds holds named constants for a count of seconds.
package seconds

// The constants have no type. Each one takes the type int64 where
// SecondsIn uses it.
const (
	secondsPerMinute = 60
	minutesPerHour   = 60
	hoursPerDay      = 24
	SecondsPerDay    = secondsPerMinute * minutesPerHour * hoursPerDay
)

// SecondsIn returns the number of seconds in days days of 24 hours.
func SecondsIn(days int64) int64 {
	return days * SecondsPerDay
}
