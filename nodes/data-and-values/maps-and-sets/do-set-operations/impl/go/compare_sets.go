// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-set-operations/.

// Package compare does set operations on two lists of words.
package compare

import (
	"maps"
	"slices"
)

// Result has the results of the set operations. Each slice is in ascending
// order.
type Result struct {
	Union, Intersection, Difference, SymmetricDifference []string
	Subset                                               bool
}

type set map[string]struct{}

func newSet(words []string) set {
	s := make(set, len(words))
	for _, w := range words {
		s[w] = struct{}{}
	}
	return s
}

func (s set) has(w string) bool {
	_, ok := s[w]
	return ok
}

// CompareSets returns the union, intersection, difference and symmetric
// difference of the sets of a and b, and whether the set of a is a subset of
// the set of b.
func CompareSets(a, b []string) Result {
	// Go has no set type and no set operations, so each one is a loop.
	sa, sb := newSet(a), newSet(b)
	union, inter, diff, sym := set{}, set{}, set{}, set{}
	for w := range sa {
		union[w] = struct{}{}
		if sb.has(w) {
			inter[w] = struct{}{}
		} else {
			diff[w] = struct{}{}
			sym[w] = struct{}{}
		}
	}
	for w := range sb {
		if !sa.has(w) {
			union[w] = struct{}{}
			sym[w] = struct{}{}
		}
	}
	sorted := func(s set) []string { return slices.Sorted(maps.Keys(s)) }
	return Result{sorted(union), sorted(inter), sorted(diff), sorted(sym), len(diff) == 0}
}
