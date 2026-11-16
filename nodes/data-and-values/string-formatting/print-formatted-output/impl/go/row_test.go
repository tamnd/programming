// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-formatted-output/.

package row

import "testing"

var cases = []struct {
	name  string
	item  string
	qty   int64
	price float64
	want  string
}{
	{"apple", "apple", 3, 1.5, "apple     |    3|    1.50"},
	{"widget", "widget", 12, 19.99, "widget    |   12|   19.99"},
	{"laptop", "laptop", 1, 1234.5, "laptop    |    1| 1234.50"},
	{"empty-name", "", 0, 0.0, "          |    0|    0.00"},
	{"exact-widths", "abcdefghij", 99999, 99999.99, "abcdefghij|99999|99999.99"},
	{"long-name", "watermelonade", 5, 2.0, "watermelonade|    5|    2.00"},
	{"wide-quantity", "pen", 123456, 0.5, "pen       |123456|    0.50"},
	{"wide-price", "car", 1, 123456.75, "car       |    1|123456.75"},
	{"negative-values", "refund", -2, -3.5, "refund    |   -2|   -3.50"},
	{"rounds-up", "nail", 1000, 0.006, "nail      | 1000|    0.01"},
	{"rounds-down", "bolt", 7, 2.344, "bolt      |    7|    2.34"},
	{"name-with-spaces", "blue pen", 4, 0.99, "blue pen  |    4|    0.99"},
	{"name-with-percent", "100% {x}", 1, 1.0, "100% {x}  |    1|    1.00"},
}

func TestFormatRow(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := FormatRow(c.item, c.qty, c.price); got != c.want {
				t.Fatalf("FormatRow(%q, %d, %v) = %q, want %q", c.item, c.qty, c.price, got, c.want)
			}
		})
	}
}
