// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-a-map/.

// Package scorelines walks the entries and the values of a map.
package scorelines

import (
	"maps"
	"slices"
	"strconv"
)

// ScoreLines returns a line key=value for each entry in ascending order of
// the key, then the line total=N with the sum of the values.
func ScoreLines(scores map[string]int) []string {
	lines := make([]string, 0, len(scores)+1)
	// The order of a range over a map is not specified, so sort the keys.
	for _, k := range slices.Sorted(maps.Keys(scores)) {
		lines = append(lines, k+"="+strconv.Itoa(scores[k]))
	}
	total := 0
	for _, v := range scores {
		total += v
	}
	return append(lines, "total="+strconv.Itoa(total))
}
