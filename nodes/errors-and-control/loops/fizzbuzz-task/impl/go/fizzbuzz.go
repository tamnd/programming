// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/fizzbuzz-task/.

// Package fizzbuzz counts from 1 to n with Fizz and Buzz.
package fizzbuzz

import "strconv"

// FizzBuzz returns FizzBuzz, Fizz, Buzz or the number for each number from 1 to n.
func FizzBuzz(n int) []string {
	out := []string{}
	for i := 1; i <= n; i++ {
		// Test 15 first: a multiple of 15 is also a multiple of 3 and of 5.
		switch {
		case i%15 == 0:
			out = append(out, "FizzBuzz")
		case i%3 == 0:
			out = append(out, "Fizz")
		case i%5 == 0:
			out = append(out, "Buzz")
		default:
			out = append(out, strconv.Itoa(i))
		}
	}
	return out
}
