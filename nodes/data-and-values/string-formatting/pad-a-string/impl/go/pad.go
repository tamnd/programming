// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pad-a-string/.

// Package pad pads a string on the left and on the right.
package pad

import "strings"

// Pad returns s padded with fill to width, once on the left and once on the
// right. If s has width bytes or more, both results are s.
func Pad(s string, width int, fill string) (left, right string) {
	extra := strings.Repeat(fill, max(0, width-len(s)))
	return extra + s, s + extra
}
