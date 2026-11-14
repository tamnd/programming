// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sleep-for-a-duration/.

// Package sleep sleeps for a duration on a monotonic clock.
package sleep

import (
	"context"
	"time"
)

// SleepLoop sleeps until clock reaches the deadline. A sleep that ends early
// is followed by a sleep for the rest of the time.
func SleepLoop(durationNs int64, clock func() int64, sleep func(ns int64)) {
	now := clock()
	deadline := now + durationNs
	for now < deadline {
		sleep(deadline - now)
		now = clock()
	}
}

// SleepFor blocks the goroutine for at least d. time.Since uses the
// monotonic clock reading of base.
func SleepFor(d time.Duration) {
	base := time.Now()
	clock := func() int64 { return int64(time.Since(base)) }
	SleepLoop(int64(d), clock, func(ns int64) { time.Sleep(time.Duration(ns)) })
}

// SleepContext waits for d or until ctx is done, whichever comes first. It
// returns ctx.Err() if ctx is done first, and nil otherwise.
func SleepContext(ctx context.Context, d time.Duration) error {
	timer := time.NewTimer(d)
	defer timer.Stop()
	select {
	case <-timer.C:
		return nil
	case <-ctx.Done():
		return ctx.Err()
	}
}

// SleepRequests runs SleepLoop on a fake clock and returns the durations it
// asks for.
func SleepRequests(durationNs, startNs int64, readings []int64) []int64 {
	times := append([]int64{startNs}, readings...)
	next := 0
	requests := []int64{}
	SleepLoop(durationNs, func() int64 {
		t := times[next]
		next++
		return t
	}, func(ns int64) { requests = append(requests, ns) })
	return requests
}
