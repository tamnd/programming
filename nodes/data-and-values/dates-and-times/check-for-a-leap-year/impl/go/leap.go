// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-for-a-leap-year/.

// Package leap checks for a leap year of the proleptic Gregorian calendar.
package leap

// IsLeapYear returns true for a leap year. The time package has no exported
// function for this rule.
func IsLeapYear(year int) bool {
	return year%4 == 0 && (year%100 != 0 || year%400 == 0)
}
