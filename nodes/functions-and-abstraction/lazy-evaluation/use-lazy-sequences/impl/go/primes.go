// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-lazy-sequences/.

// Package primes gives the prime numbers through a lazy sequence.
package primes

import "iter"

// IsPrime reports whether k is a prime number, by trial division.
func IsPrime(k int) bool {
	if k < 2 {
		return false
	}
	for d := 2; d*d <= k; d++ {
		if k%d == 0 {
			return false
		}
	}
	return true
}

// Naturals is the infinite sequence start, start + 1, ...
func Naturals(start int) iter.Seq[int] {
	return func(yield func(int) bool) {
		for i := start; ; i++ {
			if !yield(i) {
				return
			}
		}
	}
}

// Filter keeps the values of seq that pass test. It reads seq only when it is read.
func Filter(seq iter.Seq[int], test func(int) bool) iter.Seq[int] {
	return func(yield func(int) bool) {
		for v := range seq {
			if test(v) && !yield(v) {
				return
			}
		}
	}
}

// All is the infinite lazy sequence of the prime numbers.
func All() iter.Seq[int] {
	return Filter(Naturals(2), IsPrime)
}

// First returns the first n prime numbers.
func First(n int) []int {
	out := []int{}
	if n == 0 {
		return out
	}
	// The break makes yield return false, and each step of the pipeline stops.
	for p := range All() {
		out = append(out, p)
		if len(out) == n {
			break
		}
	}
	return out
}
