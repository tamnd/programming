// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-an-integer/.

// Package parseinteger parses a signed 64-bit integer with a strict syntax.
package parseinteger

import "strconv"

// ParseInteger parses s as a signed 64-bit integer in the given base, from 2
// to 36. The error is a *strconv.NumError that wraps strconv.ErrSyntax when s
// is not an optional sign and ASCII digits of the base, and strconv.ErrRange
// when the value does not fit in an int64.
func ParseInteger(s string, base int) (int64, error) {
	if base == 0 {
		// Base 0 reads a prefix such as 0x and allows underscores.
		return 0, &strconv.NumError{Func: "ParseInteger", Num: s, Err: strconv.ErrSyntax}
	}
	return strconv.ParseInt(s, base, 64)
}
