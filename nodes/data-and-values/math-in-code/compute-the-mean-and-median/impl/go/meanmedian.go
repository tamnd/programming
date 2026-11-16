// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-mean-and-median/.

// Package meanmedian computes the mean and the median of a slice of integers.
package meanmedian

import "slices"

// MeanAndMedian returns the mean and the median of a non-empty slice.
// slices.Sorted makes a sorted copy, so the slice of the caller does not
// change.
func MeanAndMedian(items []int64) (mean, median float64) {
	var sum float64
	for _, x := range items {
		sum += float64(x)
	}
	n := len(items)
	s := slices.Sorted(slices.Values(items))
	mid := n / 2
	if n%2 == 1 {
		median = float64(s[mid])
	} else {
		median = (float64(s[mid-1]) + float64(s[mid])) / 2
	}
	return sum / float64(n), median
}
