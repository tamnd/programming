// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/trim-whitespace/.

// Package asciitrim trims the ASCII white space from a string.
package asciitrim

import "strings"

// space is the 6 ASCII white space characters. strings.TrimSpace also removes
// the white space of Unicode, such as U+00A0 and U+0085.
const space = " \t\n\v\f\r"

// Trim returns s with the white space at the start and at the end removed.
func Trim(s string) string {
	return strings.Trim(s, space)
}

// TrimStart returns s with the white space at the start removed.
func TrimStart(s string) string {
	return strings.TrimLeft(s, space)
}
