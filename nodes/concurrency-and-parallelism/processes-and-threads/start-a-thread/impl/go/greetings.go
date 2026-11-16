// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/start-a-thread/.

// Package greetings starts one goroutine per name; each goroutine builds the greeting for its name.
package greetings

import "sync"

// ThreadGreetings returns "hello from " + names[i] for each i, each built on its own goroutine.
func ThreadGreetings(names []string) []string {
	results := make([]string, len(names))
	var wg sync.WaitGroup
	for i, name := range names {
		wg.Add(1)
		// Since Go 1.22, each iteration has its own i and name, so the goroutine may use them.
		go func() {
			defer wg.Done()
			results[i] = "hello from " + name
		}()
	}
	// Wait returns after every goroutine has returned; their writes are then visible.
	wg.Wait()
	return results
}
