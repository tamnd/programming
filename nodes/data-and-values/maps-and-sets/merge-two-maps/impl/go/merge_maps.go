// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/merge-two-maps/.

// Package mergemaps merges two maps into a new map.
package mergemaps

import (
	"maps"
	"slices"
)

// Pair is one entry of a map.
type Pair struct {
	Key   string
	Value int
}

// MergeMaps returns the entries of both maps in ascending order of the key.
// For a key in both maps, the value of override wins. The maps do not change.
func MergeMaps(base, override map[string]int) []Pair {
	// A map value refers to the map, so copy base before the change.
	merged := maps.Clone(base)
	if merged == nil {
		// Clone of a nil map is nil, and Copy into a nil map panics.
		merged = make(map[string]int, len(override))
	}
	// Copy replaces the values of keys that are there.
	maps.Copy(merged, override)
	out := make([]Pair, 0, len(merged))
	for _, k := range slices.Sorted(maps.Keys(merged)) {
		out = append(out, Pair{k, merged[k]})
	}
	return out
}
