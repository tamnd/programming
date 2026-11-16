// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-switch-or-match/.

// Package daysinmonth maps a month number to its number of days.
package daysinmonth

// DaysInMonth returns the number of days in a month of a common year, or 0
// for a month outside 1 to 12.
func DaysInMonth(month int) int {
	// A Go case ends by itself, so no break is needed.
	switch month {
	case 2:
		return 28
	case 4, 6, 9, 11:
		return 30
	case 1, 3, 5, 7, 8, 10, 12:
		return 31
	default:
		return 0
	}
}
