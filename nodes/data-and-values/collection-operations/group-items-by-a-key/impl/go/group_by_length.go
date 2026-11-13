// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/group-items-by-a-key/.

// Package groupbylength groups words by their length.
package groupbylength

import "slices"

// Group is the words of one length.
type Group struct {
	Len   int
	Words []string
}

// GroupByLength returns one group for each length, in ascending order of the
// length. The words of a group are in input order.
func GroupByLength(words []string) []Group {
	groups := make(map[int][]string)
	for _, w := range words {
		// append can make a new slice, so store the result in the map.
		groups[len(w)] = append(groups[len(w)], w)
	}
	keys := make([]int, 0, len(groups))
	for k := range groups {
		keys = append(keys, k)
	}
	slices.Sort(keys)
	out := make([]Group, 0, len(keys))
	for _, k := range keys {
		out = append(out, Group{k, groups[k]})
	}
	return out
}
