// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-variable/.

// Package total declares a variable, initializes it and reassigns it.
package total

// Total returns the sum of values. The var declaration without a value
// gives result the zero value 0 of its type. The short declaration := of
// the loop variable value takes its type from the slice.
func Total(values []int64) int64 {
	var result int64
	for _, value := range values {
		result = result + value
	}
	return result
}
