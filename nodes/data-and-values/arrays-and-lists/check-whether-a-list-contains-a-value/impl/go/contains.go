// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

// Package contains tells whether a slice has a value.
package contains

import "slices"

// Contains reports whether some item of items is equal to value.
func Contains(items []int, value int) bool {
	return slices.Contains(items, value)
}
