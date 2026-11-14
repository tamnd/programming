// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

// Package lookup reads keys from a map with a default value.
package lookup

// Lookup returns the value of each key in table, or fallback for a key that
// is not in table. The map does not change.
func Lookup(table map[string]int, keys []string, fallback int) []int {
	out := make([]int, 0, len(keys))
	for _, key := range keys {
		// table[key] alone gives 0 for a missing key. The second result
		// tells a missing key from a stored 0.
		v, ok := table[key]
		if !ok {
			v = fallback
		}
		out = append(out, v)
	}
	return out
}
