// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-current-date-and-time/.

package now

import (
	"math"
	"regexp"
	"strconv"
	"testing"
	"time"
)

var cases = []struct {
	name    string
	epochNs int64
	want    string
}{
	{"epoch", 0, "1970-01-01T00:00:00.000Z"},
	{"billennium", 1000000000000000000, "2001-09-09T01:46:40.000Z"},
	{"with-milliseconds", 1710054000123456789, "2024-03-10T07:00:00.123Z"},
	{"leap-day", 1709208000000000000, "2024-02-29T12:00:00.000Z"},
	{"truncate-not-round", 999999, "1970-01-01T00:00:00.000Z"},
	{"one-ns-before-epoch", -1, "1969-12-31T23:59:59.999Z"},
	{"one-ms-before-epoch", -1000000, "1969-12-31T23:59:59.999Z"},
	{"just-over-one-ms-before-epoch", -1000001, "1969-12-31T23:59:59.998Z"},
	{"last-ms-of-a-year", 1704067199999999999, "2023-12-31T23:59:59.999Z"},
	{"year-2000-is-leap", 951782400000000000, "2000-02-29T00:00:00.000Z"},
	{"year-2100-is-not-leap", 4107542399999999999, "2100-02-28T23:59:59.999Z"},
	{"after-int32-seconds", 2147483648000000000, "2038-01-19T03:14:08.000Z"},
	{"int64-max", 9223372036854775807, "2262-04-11T23:47:16.854Z"},
	{"int64-min", math.MinInt64, "1677-09-21T00:12:43.145Z"},
}

func TestUTCTimestamp(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := UTCTimestamp(c.epochNs); got != c.want {
				t.Errorf("UTCTimestamp(%d) = %q, want %q", c.epochNs, got, c.want)
			}
		})
	}
}

func TestNowUTCWithAFakeClock(t *testing.T) {
	calls := 0
	ny, err := time.LoadLocation("America/New_York")
	if err != nil {
		t.Skip("no tz database:", err)
	}
	clock := func() time.Time {
		calls++
		// The location of the time does not change the instant.
		return time.Unix(1710054000, 123456789).In(ny)
	}
	if got, want := NowUTC(clock), "2024-03-10T07:00:00.123Z"; got != want {
		t.Errorf("NowUTC = %q, want %q", got, want)
	}
	if calls != 1 {
		t.Errorf("the clock was read %d times, want 1", calls)
	}
}

func TestNowUTCWithTheRealClock(t *testing.T) {
	got := NowUTC(time.Now)
	if !regexp.MustCompile(`^\d{4}-\d\d-\d\dT\d\d:\d\d:\d\d\.\d{3}Z$`).MatchString(got) {
		t.Fatalf("NowUTC(time.Now) = %q", got)
	}
	if year, _ := strconv.Atoi(got[:4]); year < 2024 {
		t.Errorf("year %d < 2024", year)
	}
}

func TestInt64Limits(t *testing.T) {
	if UTCTimestamp(math.MaxInt64)[:4] != "2262" {
		t.Error("math.MaxInt64 is not in 2262")
	}
}
