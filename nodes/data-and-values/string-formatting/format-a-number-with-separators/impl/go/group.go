// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-separators/.

// Package group writes an integer with a separator between each group of 3
// digits.
package group

import (
	"strconv"
	"strings"
)

// GroupThousands returns n in decimal with a comma between each group of 3
// digits. It converts n to text first, so the smallest int64 does not
// overflow.
func GroupThousands(n int64) string {
	s := strconv.FormatInt(n, 10)
	sign := ""
	if s[0] == '-' {
		sign, s = "-", s[1:]
	}
	var b strings.Builder
	b.WriteString(sign)
	for i := 0; i < len(s); i++ {
		if i > 0 && (len(s)-i)%3 == 0 {
			b.WriteByte(',')
		}
		b.WriteByte(s[i])
	}
	return b.String()
}
