// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-day-of-the-week/.

// Package weekday finds the day of the week and the ISO week of a date.
package weekday

import "time"

// DayOfWeek returns the ISO 8601 day of the week: 1 for Monday to 7 for
// Sunday. time.Weekday has Sunday = 0, so 0 becomes 7.
func DayOfWeek(year, month, day int) int {
	weekday := int(time.Date(year, time.Month(month), day, 0, 0, 0, 0, time.UTC).Weekday())
	if weekday == 0 {
		return 7
	}
	return weekday
}

// ISOWeek returns the ISO 8601 week-based year and week number of a date.
func ISOWeek(year, month, day int) (int, int) {
	return time.Date(year, time.Month(month), day, 0, 0, 0, 0, time.UTC).ISOWeek()
}
