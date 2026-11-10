// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/measure-elapsed-time/.

package elapsed

import (
	"math"
	"slices"
	"testing"
	"time"
)

var cases = []struct {
	name           string
	startNs, endNs int64
	want           string
}{
	{"one-and-a-half-ms", 1000000000, 1001500000, "1.500 ms"},
	{"seconds", 0, 2345678901, "2345.678 ms"},
	{"under-one-ms", 50000, 750000, "0.700 ms"},
	{"zero", 123456789, 123456789, "0.000 ms"},
	{"under-one-microsecond", 0, 999, "0.000 ms"},
	{"truncate-not-round", 0, 1999999, "1.999 ms"},
	{"leading-zero-in-fraction", 0, 1005000, "1.005 ms"},
	{"exact-ms", 0, 7000000, "7.000 ms"},
	{"one-hour", 0, 3600000000000, "3600000.000 ms"},
	{"negative-readings", -5000000, -3500000, "1.500 ms"},
	{"across-zero", -1000000, 500000, "1.500 ms"},
	{"beyond-2-pow-53", 9007199254740993, 9007199255741992, "1.000 ms"},
	{"near-int64-max", 9223372035854775807, 9223372036854775807, "1000.000 ms"},
	{"near-int64-min", math.MinInt64, -9223372035854775808, "1000.000 ms"},
}

func TestElapsedMs(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := ElapsedMs(c.startNs, c.endNs); got != c.want {
				t.Errorf("ElapsedMs(%d, %d) = %q, want %q", c.startNs, c.endNs, got, c.want)
			}
		})
	}
}

func TestMeasureWithAFakeClock(t *testing.T) {
	var calls []string
	readings := []time.Time{time.Unix(0, 5_000_000), time.Unix(0, 6_500_000)}
	now := func() time.Time {
		calls = append(calls, "clock")
		r := readings[0]
		readings = readings[1:]
		return r
	}
	if got := Measure(func() { calls = append(calls, "work") }, now); got != 1500*time.Microsecond {
		t.Errorf("Measure = %v, want 1.5ms", got)
	}
	if want := []string{"clock", "work", "clock"}; !slices.Equal(calls, want) {
		t.Errorf("calls = %v, want %v", calls, want)
	}
}

func TestMeasureWithTheMonotonicClock(t *testing.T) {
	var calls []string
	if got := Measure(func() { calls = append(calls, "work") }, time.Now); got < 0 {
		t.Errorf("Measure = %v, want >= 0", got)
	}
	if want := []string{"work"}; !slices.Equal(calls, want) {
		t.Errorf("calls = %v, want %v", calls, want)
	}
}
