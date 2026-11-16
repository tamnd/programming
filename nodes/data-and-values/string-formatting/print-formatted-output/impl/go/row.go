// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-formatted-output/.

// Package row formats one line of a table with fixed columns.
package row

import "fmt"

// FormatRow returns name in 10 columns on the left, qty in 5 on the right
// and price with 2 decimals in 8 on the right. A width is a minimum, so a
// long value is not cut.
func FormatRow(name string, qty int64, price float64) string {
	return fmt.Sprintf("%-10s|%5d|%8.2f", name, qty, price)
}
