// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/partition-a-list/.

// Package partitionodd splits a slice into its odd and even items.
package partitionodd

// PartitionOdd returns the odd items and the even items, each in input order.
// It makes new slices, so the input does not change.
func PartitionOdd(items []int) (odd, even []int) {
	for _, x := range items {
		if x%2 != 0 {
			odd = append(odd, x)
		} else {
			even = append(even, x)
		}
	}
	return odd, even
}
