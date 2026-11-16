// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sum-and-multiply-a-list/.

// Package sumprod computes the sum and the product of a slice of integers.
package sumprod

// SumAndProduct returns the sum and the product of the items. Go has no
// sum function for a slice, and the operations wrap around on overflow.
func SumAndProduct(items []int64) (sum, product int64) {
	product = 1
	for _, x := range items {
		sum += x
		product *= x
	}
	return sum, product
}
