// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-map/.

// Package buildmap creates a map, changes it and lists its entries.
package buildmap

import (
	"maps"
	"slices"
)

// Pair is one entry of the map.
type Pair struct {
	Key   string
	Value int
}

// BuildMap puts the pairs in a map, deletes the keys in remove, and returns
// the entries in ascending order of the key.
func BuildMap(pairs []Pair, remove []string) []Pair {
	// make gives an empty map. A nil map panics on an assignment.
	m := make(map[string]int)
	for _, p := range pairs {
		// An assignment adds the key, or replaces the value of a key that is there.
		m[p.Key] = p.Value
	}
	for _, k := range remove {
		// delete does nothing for a key that is not there.
		delete(m, k)
	}
	out := make([]Pair, 0, len(m))
	for _, k := range slices.Sorted(maps.Keys(m)) {
		out = append(out, Pair{k, m[k]})
	}
	return out
}
