// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-equality-for-a-type/.

// Package money shows equality for a Go struct. The fields are comparable, so == compares
// them, and a Money value can be a map key. The constructor makes the equal values identical.
package money

import "strings"

// Money is an amount in cents and an upper-case currency code.
type Money struct {
	cents    int64
	currency string
}

// New returns a Money value. "eur" and "EUR" are the same currency.
func New(cents int64, currency string) Money {
	return Money{cents: cents, currency: strings.ToUpper(currency)}
}

// Equal reports whether m and other have the same amount and currency.
func (m Money) Equal(other Money) bool {
	return m == other
}

// CountDistinct returns the number of different Money values.
func CountDistinct(amounts []int64, currencies []string) int {
	seen := make(map[Money]struct{}, len(amounts))
	for i, amount := range amounts {
		seen[New(amount, currencies[i])] = struct{}{}
	}
	return len(seen)
}
