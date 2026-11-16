// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-higher-order-function/.

// Package higherorder has a function that takes a function and returns a function.
package higherorder

// Repeat returns a function that applies f to its argument times times.
func Repeat(f func(int) int, times int) func(int) int {
	return func(x int) int {
		for i := 0; i < times; i++ {
			x = f(x)
		}
		return x
	}
}

// CollatzStep returns n / 2 for an even n and 3n + 1 for an odd n.
func CollatzStep(n int) int {
	if n%2 == 0 {
		return n / 2
	}
	return 3*n + 1
}

// CollatzAfter returns the value after times Collatz steps from start.
func CollatzAfter(start, times int) int {
	return Repeat(CollatzStep, times)(start)
}
