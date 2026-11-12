// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

// Package dedupe removes duplicates from slices and keeps the input order.
package dedupe

import "strings"

// User is a record of a user.
type User struct {
	Name  string
	Email string
}

// Unique returns a new slice of the items without later copies.
// The first copies keep their order.
func Unique[T comparable](items []T) []T {
	return UniqueBy(items, func(x T) T { return x })
}

// UniqueBy returns a new slice of the first item for each key, in input order.
func UniqueBy[T any, K comparable](items []T, key func(T) K) []T {
	// The map only records the keys that we saw. The order comes from the
	// slice, because the iteration order of a map is not specified.
	seen := make(map[K]struct{}, len(items))
	out := make([]T, 0, len(items))
	for _, item := range items {
		k := key(item)
		if _, ok := seen[k]; ok {
			continue
		}
		seen[k] = struct{}{}
		out = append(out, item)
	}
	return out
}

// UniqueByEmail returns the first user for each email.
// Case does not matter in the emails, which are ASCII.
func UniqueByEmail(users []User) []User {
	return UniqueBy(users, func(u User) string { return strings.ToLower(u.Email) })
}
