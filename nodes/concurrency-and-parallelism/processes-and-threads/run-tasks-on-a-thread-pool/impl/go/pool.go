// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-tasks-on-a-thread-pool/.

// Package pool sends one job for each number to a fixed set of worker goroutines
// and collects the results in order.
package pool

import "sync"

func collatzSteps(n int64) int {
	steps := 0
	for n != 1 {
		if n%2 == 0 {
			n /= 2
		} else {
			n = 3*n + 1
		}
		steps++
	}
	return steps
}

// StepsOnPool returns the step count of each number, in the order of numbers.
func StepsOnPool(workers int, numbers []int) []int {
	results := make([]int, len(numbers))
	jobs := make(chan int) // indexes into numbers
	var wg sync.WaitGroup
	for range workers {
		wg.Add(1)
		go func() {
			defer wg.Done()
			// The loop ends when jobs is closed and empty.
			for i := range jobs {
				results[i] = collatzSteps(int64(numbers[i])) // each job writes its own element
			}
		}()
	}
	for i := range numbers {
		jobs <- i
	}
	close(jobs)
	wg.Wait()
	return results
}
