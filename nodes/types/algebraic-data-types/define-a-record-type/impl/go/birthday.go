// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-record-type/.

// Package birthday defines a record type as a Go struct.
package birthday

// Person is a struct. Assignment copies all its fields, and == compares them.
type Person struct {
	Name string
	Age  int
}

// Birthday returns a new slice where each person called name is one year older.
func Birthday(people []Person, name string) []Person {
	out := make([]Person, len(people))
	for i, p := range people {
		if p.Name == name {
			p.Age++ // p is a copy, so people[i] does not change
		}
		out[i] = p
	}
	return out
}
