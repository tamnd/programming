// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/build-a-string-efficiently/.

// Package numberline builds the line of numbers 1 to n.
package numberline

import (
	"strconv"
	"strings"
)

// NumberLine returns the numbers 1 to n, separated by commas.
func NumberLine(n int) string {
	// A strings.Builder appends to one byte slice, and String returns it
	// with no copy.
	var b strings.Builder
	for i := 1; i <= n; i++ {
		if i > 1 {
			b.WriteByte(',')
		}
		b.WriteString(strconv.Itoa(i))
	}
	return b.String()
}
