// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-while-loop/.

// Package halvings halves a number with a while loop.
package halvings

// Halvings returns n, n/2, n/4, ... while the value is greater than 0.
func Halvings(n int) []int {
	out := []int{}
	// A for with only a condition is the while loop of Go.
	for n > 0 {
		out = append(out, n)
		n /= 2
	}
	return out
}
