// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wait-for-threads-to-finish/.

// Package sums sums each chunk on its own goroutine, waits for all of them and collects the sums.
package sums

import "sync"

// ParallelSums returns the sum of each chunk, in the order of chunks.
func ParallelSums(chunks [][]int64) []int64 {
	sums := make([]int64, len(chunks))
	var wg sync.WaitGroup
	for i, chunk := range chunks {
		// Add counts the goroutine before it starts; Done uncounts it when it returns.
		wg.Add(1)
		go func() {
			defer wg.Done()
			var sum int64
			for _, n := range chunk {
				sum += n
			}
			sums[i] = sum
		}()
	}
	// Wait blocks until the counter is zero, that is, until every goroutine has called Done.
	wg.Wait()
	return sums
}
