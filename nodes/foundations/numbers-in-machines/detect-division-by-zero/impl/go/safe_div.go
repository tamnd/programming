// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-division-by-zero/.

// Package division divides two int64 values and reports a zero divisor.
package division

import "math"

// SafeDiv returns a / b rounded toward zero and true. It returns 0 and false
// when b is 0, and when a is math.MinInt64 and b is -1.
func SafeDiv(a, b int64) (int64, bool) {
	// A zero divisor at run time causes a panic. math.MinInt64 / -1 does not
	// panic: the quotient wraps to math.MinInt64. So test both first.
	if b == 0 || (a == math.MinInt64 && b == -1) {
		return 0, false
	}
	return a / b, true
}
