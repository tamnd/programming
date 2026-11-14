// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-days-to-a-date/.

package add

import "time"

// DaysIn returns the number of days in a month of a year.
func DaysIn(year int, month time.Month) int {
	// Day 0 of the next month is the last day of this month.
	return time.Date(year, month+1, 0, 0, 0, 0, 0, time.UTC).Day()
}

// AddToDate adds years and months at once, then days, and returns the date as
// YYYY-MM-DD. A day after the end of the new month becomes its last day.
// AddDate alone would move such a day into the next month.
func AddToDate(year, month, day, years, months, days int) string {
	total := year*12 + (month - 1) + years*12 + months
	newYear, newMonth := total/12, time.Month(total%12+1)
	newDay := min(day, DaysIn(newYear, newMonth))
	moved := time.Date(newYear, newMonth, newDay, 0, 0, 0, 0, time.UTC)
	return moved.AddDate(0, 0, days).Format(time.DateOnly)
}
