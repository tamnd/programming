// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-set/.

// Package toggle builds a set of words.
package toggle

import (
	"maps"
	"slices"
)

// Toggle adds each word that is not in the set, and removes each word that is
// in it. It returns the words of the set in ascending order.
func Toggle(words []string) []string {
	// Go has no set type. A map with values of the empty struct type is a set,
	// and the values take no memory.
	seen := make(map[string]struct{})
	for _, word := range words {
		if _, ok := seen[word]; ok {
			delete(seen, word)
		} else {
			seen[word] = struct{}{}
		}
	}
	return slices.Sorted(maps.Keys(seen))
}
