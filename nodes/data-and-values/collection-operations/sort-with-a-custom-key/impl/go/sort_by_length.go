// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-with-a-custom-key/.

// Package sortbylength sorts words by their length, then by the word.
package sortbylength

import (
	"cmp"
	"slices"
	"strings"
)

// SortByLength returns a sorted copy of words: in order of the length, then
// of the bytes of the word.
func SortByLength(words []string) []string {
	out := slices.Clone(words)
	slices.SortFunc(out, func(a, b string) int {
		// cmp.Or gives the first result that is not 0.
		return cmp.Or(cmp.Compare(len(a), len(b)), strings.Compare(a, b))
	})
	return out
}
