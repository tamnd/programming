// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/interpolate-values-into-a-string/.

// Package order builds a sentence from the values of an order.
package order

import "fmt"

// DescribeOrder returns "Order {name}: count item(s), total count*price".
// Go has no string interpolation, so this uses fmt.Sprintf.
func DescribeOrder(name string, count int64, price float64) string {
	ending := "s"
	if count == 1 {
		ending = ""
	}
	return fmt.Sprintf("Order {%s}: %d item%s, total %.2f", name, count, ending, float64(count)*price)
}
