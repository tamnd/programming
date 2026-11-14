// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-difference-of-two-dates/.

// Package diff counts the days between two dates.
package diff

import "time"

// DaysBetween returns the number of days from the start date to the end date.
// It subtracts Unix seconds and not two times with Sub, because a
// time.Duration holds about 292 years at most.
func DaysBetween(startYear, startMonth, startDay, endYear, endMonth, endDay int) int64 {
	start := time.Date(startYear, time.Month(startMonth), startDay, 0, 0, 0, 0, time.UTC)
	end := time.Date(endYear, time.Month(endMonth), endDay, 0, 0, 0, 0, time.UTC)
	return (end.Unix() - start.Unix()) / (24 * 60 * 60)
}
