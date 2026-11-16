// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-conditional/.

// Package grade maps a score to a letter grade.
package grade

// Grade returns the letter grade for a score from 0 to 100.
func Grade(score int) string {
	if score >= 90 {
		return "A"
	} else if score >= 80 {
		return "B"
	} else if score >= 70 {
		return "C"
	} else if score >= 60 {
		return "D"
	} else {
		return "F"
	}
}
