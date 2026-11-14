// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-for-a-leap-year/.

package leap

import (
	"testing"
	"time"
)

var cases = []struct {
	name string
	year int
	want bool
}{
	{"divisible-by-4", 2024, true},
	{"not-divisible-by-4", 2023, false},
	{"century", 1900, false},
	{"divisible-by-400", 2000, true},
	{"next-century", 2100, false},
	{"year-1600", 1600, true},
	{"year-1700", 1700, false},
	{"year-2400", 2400, true},
	{"year-1", 1, false},
	{"year-4", 4, true},
	{"year-100", 100, false},
	{"year-1500-julian-leap", 1500, false},
	{"year-1582", 1582, false},
	{"year-9999", 9999, false},
	{"year-zero", 0, true},
	{"year-minus-1", -1, false},
	{"year-minus-4", -4, true},
	{"year-minus-100", -100, false},
	{"year-minus-400", -400, true},
}

func TestIsLeapYear(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := IsLeapYear(c.year); got != c.want {
				t.Fatalf("got %v, want %v", got, c.want)
			}
		})
	}
}

// The last day of a leap year is day 366 of the year.
func TestSameAsYearDay(t *testing.T) {
	for year := -9999; year <= 9999; year++ {
		last := time.Date(year, time.December, 31, 0, 0, 0, 0, time.UTC)
		if got := last.YearDay() == 366; got != IsLeapYear(year) {
			t.Fatalf("%d: YearDay %d", year, last.YearDay())
		}
	}
}
