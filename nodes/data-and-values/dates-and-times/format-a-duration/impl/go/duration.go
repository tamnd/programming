// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-duration/.

// Package duration formats a duration in seconds as HH:MM:SS.
package duration

import "fmt"

// FormatDuration returns the duration as HH:MM:SS. The hours do not wrap at
// 24. A time.Duration holds only about 290 years, so the function takes the
// seconds as an int64.
func FormatDuration(seconds int64) string {
	sign := ""
	a := seconds
	if seconds < 0 {
		sign, a = "-", -seconds
	}
	return fmt.Sprintf("%s%02d:%02d:%02d", sign, a/3600, a%3600/60, a%60)
}
