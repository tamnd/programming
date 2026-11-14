// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sleep-for-a-duration/.

package sleep

import (
	"context"
	"errors"
	"slices"
	"testing"
	"time"
)

var cases = []struct {
	name                string
	durationNs, startNs int64
	readings, want      []int64
}{
	{"one-call", 1000000, 0, []int64{1000000}, []int64{1000000}},
	{"woke-late", 1000000, 0, []int64{1080000}, []int64{1000000}},
	{"woke-early", 1000000, 0, []int64{600000, 1000000}, []int64{1000000, 400000}},
	{"one-second", 1000000000, 5000000000, []int64{6000050000}, []int64{1000000000}},
	{"zero", 0, 0, []int64{}, []int64{}},
	{"negative", -5000000, 0, []int64{}, []int64{}},
	{"most-negative", -4611686018427387904, 0, []int64{}, []int64{}},
	{"several-early-wakes", 10000000, 0, []int64{2000000, 5000000, 9999999, 10000000}, []int64{10000000, 8000000, 5000000, 1}},
	{"no-progress", 1000000, 0, []int64{0, 1000000}, []int64{1000000, 1000000}},
	{"exact-deadline", 3000000, 100, []int64{3000100}, []int64{3000000}},
	{"one-ns-short", 3000000, 0, []int64{2999999, 3000000}, []int64{3000000, 1}},
	{"extra-readings", 1000000, 0, []int64{1000000, 5000000}, []int64{1000000}},
	{"negative-origin", 2000000000, -5000000000, []int64{-4000000000, -3000000000}, []int64{2000000000, 1000000000}},
	{"origin-after-100-days", 1500000, 8640000000000000, []int64{8640000001000000, 8640000001500000}, []int64{1500000, 500000}},
	{"one-nanosecond", 1, 0, []int64{1000000}, []int64{1}},
	{"sub-millisecond", 1500000, 0, []int64{1000000, 1500000}, []int64{1500000, 500000}},
	{"one-hour", 3600000000000, 0, []int64{3600000000000}, []int64{3600000000000}},
	{"thirty-days", 2592000000000000, 0, []int64{2147483647000000, 2592000000000000}, []int64{2592000000000000, 444516353000000}},
}

func TestSleepRequests(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := SleepRequests(c.durationNs, c.startNs, c.readings); !slices.Equal(got, c.want) {
				t.Errorf("SleepRequests(%d, %d, %v) = %v, want %v", c.durationNs, c.startNs, c.readings, got, c.want)
			}
		})
	}
}

func TestSleepForWaitsAtLeastTheDuration(t *testing.T) {
	start := time.Now()
	SleepFor(20 * time.Millisecond)
	if e := time.Since(start); e < 20*time.Millisecond {
		t.Errorf("SleepFor(20ms) took %v", e)
	}
}

func TestSleepForReturnsAtOnce(t *testing.T) {
	for _, d := range []time.Duration{0, -5 * time.Millisecond} {
		start := time.Now()
		SleepFor(d)
		if e := time.Since(start); e >= time.Second {
			t.Errorf("SleepFor(%v) took %v", d, e)
		}
	}
}

func TestSleepContextStopsWhenCanceled(t *testing.T) {
	ctx, cancel := context.WithTimeout(context.Background(), 20*time.Millisecond)
	defer cancel()
	start := time.Now()
	err := SleepContext(ctx, time.Hour)
	if !errors.Is(err, context.DeadlineExceeded) || time.Since(start) >= time.Second {
		t.Errorf("SleepContext = %v after %v", err, time.Since(start))
	}
	if err := SleepContext(context.Background(), time.Millisecond); err != nil {
		t.Errorf("SleepContext(1ms) = %v", err)
	}
}
