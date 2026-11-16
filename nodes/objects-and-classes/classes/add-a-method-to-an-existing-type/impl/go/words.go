// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.

// Package words adds a method to a string type. Go allows a method only on a
// type declared in the same package, so the package declares Text, a new type
// whose underlying type is string, and puts the method on it.
package words

import "strings"

// Text is a string with methods. Convert with Text(s) and string(t).
type Text string

// WordCount returns the number of runs of characters that are not whitespace.
func (t Text) WordCount() int {
	return len(strings.Fields(string(t)))
}

// WordCount counts the words of a plain string through Text.
func WordCount(text string) int {
	return Text(text).WordCount()
}
