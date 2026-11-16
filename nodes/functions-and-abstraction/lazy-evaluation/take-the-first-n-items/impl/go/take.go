// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/take-the-first-n-items/.

// Package take takes the first n items of a slice or of an infinite sequence.
package take

import (
	"iter"
	"slices"
)

// Take returns a copy of the first n items; a shorter slice gives all its items.
func Take(items []int, n int) []int {
	// items[:n] panics when n is larger than the length, so use the smaller one.
	return slices.Clone(items[:min(n, len(items))])
}

// Naturals is the infinite sequence start, start + 1, ... It runs only when it
// is read, and it stops when yield returns false.
func Naturals(start int) iter.Seq[int] {
	return func(yield func(int) bool) {
		for i := start; ; i++ {
			if !yield(i) {
				return
			}
		}
	}
}

// TakeNaturals returns the first n items of the infinite sequence.
func TakeNaturals(start, n int) []int {
	out := []int{}
	if n == 0 {
		return out
	}
	for i := range Naturals(start) {
		out = append(out, i)
		if len(out) == n {
			break
		}
	}
	return out
}
