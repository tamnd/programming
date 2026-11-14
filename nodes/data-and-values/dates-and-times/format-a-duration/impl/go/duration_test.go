// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-duration/.

package duration

import (
	"testing"
	"time"
)

var cases = []struct {
	name    string
	seconds int64
	want    string
}{
	{"zero", 0, "00:00:00"},
	{"seconds-only", 59, "00:00:59"},
	{"one-minute", 60, "00:01:00"},
	{"one-hour", 3600, "01:00:00"},
	{"hours-minutes-seconds", 3723, "01:02:03"},
	{"just-under-an-hour", 3599, "00:59:59"},
	{"just-under-a-day", 86399, "23:59:59"},
	{"one-day", 86400, "24:00:00"},
	{"more-than-a-day", 90061, "25:01:01"},
	{"hundred-hours", 360000, "100:00:00"},
	{"one-week", 604800, "168:00:00"},
	{"one-year", 31536000, "8760:00:00"},
	{"minus-one-second", -1, "-00:00:01"},
	{"negative-minute", -61, "-00:01:01"},
	{"negative-hours", -3723, "-01:02:03"},
	{"negative-day", -90061, "-25:01:01"},
	{"beyond-go-duration", 10000000000, "2777777:46:40"},
	{"largest", 1000000000000000, "277777777777:46:40"},
	{"most-negative", -1000000000000000, "-277777777777:46:40"},
}

func TestFormatDuration(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := FormatDuration(c.seconds); got != c.want {
				t.Errorf("got %s, want %s", got, c.want)
			}
		})
	}
}

func TestDurationString(t *testing.T) {
	if got := (90061 * time.Second).String(); got != "25h1m1s" {
		t.Errorf("got %s", got)
	}
}
