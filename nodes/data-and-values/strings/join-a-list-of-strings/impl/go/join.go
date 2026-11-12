// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/join-a-list-of-strings/.

// Package join joins a list of strings, and a list of numbers, with a separator.
package join

import (
	"strconv"
	"strings"
)

// Join returns the items with sep between each two.
func Join(items []string, sep string) string {
	return strings.Join(items, sep)
}

// JoinNumbers returns the decimal form of each item, with sep between each two.
func JoinNumbers(items []int32, sep string) string {
	// strings.Join takes only a []string, so the code converts each item.
	parts := make([]string, len(items))
	for i, n := range items {
		parts[i] = strconv.Itoa(int(n))
	}
	return strings.Join(parts, sep)
}
