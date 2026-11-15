// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-counted-loop/.

// Package countto counts from 1 to n with a loop.
package countto

// CountTo returns the slice [1, 2, ..., n].
func CountTo(n int) []int {
	out := make([]int, 0, n)
	// "for i := range n" would count from 0 to n-1.
	for i := 1; i <= n; i++ {
		out = append(out, i)
	}
	return out
}
