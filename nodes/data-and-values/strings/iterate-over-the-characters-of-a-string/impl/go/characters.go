// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.

// Package characters gives the code points of a string.
package characters

// Characters returns the code points of s. A range loop over a string decodes
// one rune at each step. An index loop, s[i], would give single bytes.
func Characters(s string) []string {
	out := []string{}
	for _, r := range s {
		out = append(out, string(r))
	}
	return out
}
