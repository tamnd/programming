// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/return-multiple-values/.

// Package divmod returns two results from one function.
package divmod

// DivMod returns the quotient and the remainder of the floor division of a by b.
func DivMod(a, b int32) (int32, int32) {
	// The Go operators / and % truncate toward zero. Move a result that is
	// not exact one step down, so that the quotient is the floor.
	q, r := a/b, a%b
	if r != 0 && (r < 0) != (b < 0) {
		q--
		r += b
	}
	return q, r
}
