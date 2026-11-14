// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-difference-of-two-dates/.

package diff

import (
	"math"
	"testing"
	"time"
)

var cases = []struct {
	name                                                       string
	startYear, startMonth, startDay, endYear, endMonth, endDay int
	want                                                       int64
}{
	{"same-day", 2024, 3, 10, 2024, 3, 10, 0},
	{"next-day", 2024, 3, 10, 2024, 3, 11, 1},
	{"common-year", 2023, 1, 1, 2024, 1, 1, 365},
	{"leap-year", 2024, 1, 1, 2025, 1, 1, 366},
	{"end-before-start", 2024, 3, 11, 2024, 3, 10, -1},
	{"across-a-dst-change", 2024, 3, 9, 2024, 3, 11, 2},
	{"across-leap-day", 2024, 2, 28, 2024, 3, 1, 2},
	{"month-end-to-next-month", 2024, 1, 31, 2024, 3, 1, 30},
	{"year-1900-is-not-leap", 1900, 1, 1, 1901, 1, 1, 365},
	{"year-2000-is-leap", 2000, 1, 1, 2001, 1, 1, 366},
	{"400-years", 2000, 1, 1, 2400, 1, 1, 146097},
	{"across-the-epoch", 1969, 12, 31, 1970, 1, 1, 1},
	{"gregorian-reform-gap", 1582, 10, 4, 1582, 10, 15, 11},
	{"more-than-292-years", 1700, 1, 1, 2300, 1, 1, 219145},
	{"whole-range", 1, 1, 1, 9999, 12, 31, 3652058},
	{"whole-range-backward", 9999, 12, 31, 1, 1, 1, -3652058},
	{"year-99", 99, 12, 31, 100, 1, 1, 1},
}

func TestDaysBetween(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := DaysBetween(c.startYear, c.startMonth, c.startDay, c.endYear, c.endMonth, c.endDay); got != c.want {
				t.Fatalf("got %d, want %d", got, c.want)
			}
		})
	}
}

func TestSubSaturates(t *testing.T) {
	start := time.Date(1700, time.January, 1, 0, 0, 0, 0, time.UTC)
	end := time.Date(2300, time.January, 1, 0, 0, 0, 0, time.UTC)
	if got := end.Sub(start); got != time.Duration(math.MaxInt64) {
		t.Fatalf("got %v", got)
	}
}
