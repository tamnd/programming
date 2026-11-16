// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-big-integers/.

// Package bigpow computes exact integer powers with math/big.
package bigpow

import "math/big"

// BigPower returns base to the power exp as decimal text.
// A big.Int has no fixed width, so the result is exact.
func BigPower(base, exp int) string {
	return new(big.Int).Exp(big.NewInt(int64(base)), big.NewInt(int64(exp)), nil).String()
}
