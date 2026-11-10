// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/measure-elapsed-time/.

// Package elapsed measures the time that a piece of work takes.
package elapsed

import (
	"fmt"
	"time"
)

// Measure runs work once and returns the time it took. now is the clock, and
// time.Now is the real one. A Time from time.Now holds a monotonic clock
// reading, and Sub uses it when both times have one.
func Measure(work func(), now func() time.Time) time.Duration {
	start := now()
	work()
	return now().Sub(start)
}

// ElapsedMs returns endNs - startNs as "M.UUU ms", with the rest under one
// microsecond dropped.
func ElapsedMs(startNs, endNs int64) string {
	ns := endNs - startNs
	// Integer division truncates, and ns is not negative.
	return fmt.Sprintf("%d.%03d ms", ns/1_000_000, ns/1_000%1_000)
}
