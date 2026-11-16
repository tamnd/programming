// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-thread-local-storage/.

// Package tls gives each goroutine its own counter. Go has no thread-local or goroutine-local
// storage, so each goroutine makes its counter and passes it to the code that uses it.
package tls

import "sync"

// Counter belongs to one goroutine, so it needs no lock.
type Counter struct {
	n int
}

// Bump adds 1 to the counter and returns the new value.
func (c *Counter) Bump() int {
	c.n++
	return c.n
}

// Current returns the value of the counter.
func (c *Counter) Current() int {
	return c.n
}

// CountPerThread starts one goroutine for each count and returns the final counter of each goroutine, in order.
func CountPerThread(counts []int) []int {
	results := make([]int, len(counts))
	var wg sync.WaitGroup
	for i, n := range counts {
		wg.Add(1)
		go func() {
			defer wg.Done()
			// The counter is a local variable of this goroutine. Code that needs it gets it as a value.
			var c Counter
			for range n {
				c.Bump()
			}
			// Each goroutine writes only its own element. Wait makes the write visible to the caller.
			results[i] = c.Current()
		}()
	}
	wg.Wait()
	return results
}
