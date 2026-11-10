// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/split-a-string/.

// Package split splits strings on a literal separator, on white space and on
// a regular expression.
package split

import (
	"regexp"
	"strings"
)

// Split splits s on the literal separator sep and keeps the empty fields.
// With limit n > 0 the result has at most n fields, and the last field holds
// the rest of s. A limit of 0 means no limit.
func Split(s, sep string, limit int) []string {
	if limit == 0 {
		return strings.Split(s, sep)
	}
	return strings.SplitN(s, sep, limit)
}

// SplitWhitespace splits s on runs of white space. The result has no empty
// fields.
func SplitWhitespace(s string) []string {
	return strings.Fields(s)
}

// SplitRegex splits s on each match of the regular expression pattern.
func SplitRegex(s, pattern string) ([]string, error) {
	re, err := regexp.Compile(pattern)
	if err != nil {
		return nil, err
	}
	return re.Split(s, -1), nil
}
