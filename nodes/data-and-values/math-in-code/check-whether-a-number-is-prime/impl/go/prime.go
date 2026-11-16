// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-number-is-prime/.

// Package prime tests whether an integer is prime.
package prime

// IsPrime reports whether n is prime, with trial division. i <= n/i does
// not overflow, but i*i can.
func IsPrime(n int64) bool {
	if n < 2 {
		return false
	}
	if n%2 == 0 {
		return n == 2
	}
	for i := int64(3); i <= n/i; i += 2 {
		if n%i == 0 {
			return false
		}
	}
	return true
}
