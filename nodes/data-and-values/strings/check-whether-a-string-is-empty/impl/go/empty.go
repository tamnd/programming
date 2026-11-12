// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

// Package empty checks whether a string is empty or has only ASCII white space.
package empty

import "strings"

// IsEmpty reports whether s has no bytes.
func IsEmpty(s string) bool {
	return s == ""
}

// IsBlank reports whether each byte of s is ASCII white space. strings.TrimSpace
// also removes Unicode spaces, so the code gives the six bytes to Trim.
func IsBlank(s string) bool {
	return strings.Trim(s, " \t\n\v\f\r") == ""
}
