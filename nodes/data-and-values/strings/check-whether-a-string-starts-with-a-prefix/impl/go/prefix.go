// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-starts-with-a-prefix/.

// Package prefix checks whether a string starts or ends with another string.
package prefix

import "strings"

// StartsWith reports whether s starts with prefix.
func StartsWith(s, prefix string) bool {
	return strings.HasPrefix(s, prefix)
}

// EndsWith reports whether s ends with suffix.
func EndsWith(s, suffix string) bool {
	return strings.HasSuffix(s, suffix)
}
