// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-fixed-decimals/.

// Package fixed writes a number with a fixed count of decimals.
package fixed

import "strconv"

// Fixed returns x with exactly digits digits after the decimal point. It
// rounds the exact binary value of x.
func Fixed(x float64, digits int) string {
	return strconv.FormatFloat(x, 'f', digits, 64)
}
