// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-integer-overflow/.

// Package overflow adds two int64 values and reports an overflow.
package overflow

// CheckedAdd returns a + b and true, or 0 and false when the sum does not fit
// in an int64.
func CheckedAdd(a, b int64) (int64, bool) {
	// In Go a signed overflow is defined: the sum wraps and there is no panic.
	// So compute the sum first and test it after. The sum overflowed when a
	// and b have the same sign and the sum has the other sign.
	s := a + b
	if (a >= 0) == (b >= 0) && (s >= 0) != (a >= 0) {
		return 0, false
	}
	return s, true
}
