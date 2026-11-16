// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/swap-two-values/.

// Package swap swaps two values with a tuple assignment.
package swap

import "slices"

// SwapItems returns a copy of xs with the items at i and j swapped. Go
// computes all values on the right before it assigns any of them.
func SwapItems(xs []int64, i, j int) []int64 {
	out := slices.Clone(xs)
	out[i], out[j] = out[j], out[i]
	return out
}
