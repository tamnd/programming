// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-stack/.

// Package brackets checks brackets with a stack.
package brackets

var opening = map[rune]rune{')': '(', ']': '[', '}': '{'}

// Balanced returns true if the brackets of text match and nest correctly.
// Go has no stack type. A slice is the stack: append pushes, and a shorter
// slice pops.
func Balanced(text string) bool {
	var stack []rune
	for _, c := range text {
		switch c {
		case '(', '[', '{':
			stack = append(stack, c)
		case ')', ']', '}':
			n := len(stack)
			if n == 0 || stack[n-1] != opening[c] { // stack[n-1] is the top
				return false
			}
			stack = stack[:n-1]
		}
	}
	return len(stack) == 0
}
