// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-between-time-zones/.

package tzconv

import "testing"

var cases = []struct {
	name                                   string
	year, month, day, hour, minute, second int
	fromZone, toZone                       string
	want                                   string
}{
	{"new-york-to-utc", 2024, 7, 1, 12, 0, 0, "America/New_York", "UTC", "2024-07-01T16:00:00+00:00"},
	{"utc-to-tokyo", 2024, 1, 15, 0, 0, 0, "UTC", "Asia/Tokyo", "2024-01-15T09:00:00+09:00"},
	{"london-to-new-york", 2024, 1, 15, 12, 0, 0, "Europe/London", "America/New_York", "2024-01-15T07:00:00-05:00"},
	{"date-changes", 2024, 1, 15, 22, 0, 0, "America/Los_Angeles", "Asia/Tokyo", "2024-01-16T15:00:00+09:00"},
	{"same-zone", 2024, 3, 10, 12, 0, 0, "America/New_York", "America/New_York", "2024-03-10T12:00:00-04:00"},
	{"nonexistent-time", 2024, 3, 10, 2, 30, 0, "America/New_York", "UTC", "2024-03-10T07:30:00+00:00"},
	{"ambiguous-time", 2024, 11, 3, 1, 30, 0, "America/New_York", "UTC", "2024-11-03T05:30:00+00:00"},
	{"ambiguous-time-london", 2024, 10, 27, 1, 30, 0, "Europe/London", "UTC", "2024-10-27T00:30:00+00:00"},
	{"dst-dates-differ", 2024, 3, 20, 12, 0, 0, "America/New_York", "Europe/London", "2024-03-20T16:00:00+00:00"},
	{"southern-hemisphere", 2024, 1, 15, 12, 0, 0, "Australia/Sydney", "UTC", "2024-01-15T01:00:00+00:00"},
	{"half-hour-offset", 2024, 1, 15, 12, 0, 0, "Asia/Kolkata", "UTC", "2024-01-15T06:30:00+00:00"},
	{"45-minute-offset", 2024, 1, 15, 12, 0, 0, "UTC", "Asia/Kathmandu", "2024-01-15T17:45:00+05:45"},
	{"half-hour-dst", 2024, 1, 15, 12, 0, 0, "Australia/Lord_Howe", "UTC", "2024-01-15T01:00:00+00:00"},
	{"across-the-date-line", 2024, 1, 15, 12, 0, 0, "Pacific/Kiritimati", "Pacific/Pago_Pago", "2024-01-14T11:00:00-11:00"},
	{"before-skipped-day", 2011, 12, 29, 12, 0, 0, "Pacific/Apia", "UTC", "2011-12-29T22:00:00+00:00"},
	{"skipped-day", 2011, 12, 30, 12, 0, 0, "Pacific/Apia", "UTC", "2011-12-30T22:00:00+00:00"},
	{"after-skipped-day", 2011, 12, 31, 12, 0, 0, "Pacific/Apia", "UTC", "2011-12-30T22:00:00+00:00"},
	{"old-dst-rule", 2006, 3, 15, 12, 0, 0, "America/New_York", "UTC", "2006-03-15T17:00:00+00:00"},
	{"posix-sign", 2024, 1, 15, 12, 0, 0, "UTC", "Etc/GMT+5", "2024-01-15T07:00:00-05:00"},
	{"result-before-epoch", 1970, 1, 1, 0, 0, 0, "UTC", "America/New_York", "1969-12-31T19:00:00-05:00"},
	{"far-future", 2050, 7, 1, 12, 0, 0, "America/New_York", "UTC", "2050-07-01T16:00:00+00:00"},
}

func TestConvertTimeZone(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got, err := ConvertTimeZone(c.year, c.month, c.day, c.hour, c.minute, c.second, c.fromZone, c.toZone)
			if err != nil {
				t.Fatal(err)
			}
			if got != c.want {
				t.Errorf("got %s, want %s", got, c.want)
			}
		})
	}
}

func TestUnknownZone(t *testing.T) {
	if _, err := ConvertTimeZone(2024, 1, 1, 0, 0, 0, "Mars/Olympus", "UTC"); err == nil {
		t.Fatal("no error for an unknown zone")
	}
}
