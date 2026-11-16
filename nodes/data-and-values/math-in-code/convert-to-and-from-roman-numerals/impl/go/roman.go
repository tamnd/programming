// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

// Package roman converts between integers and Roman numerals in the standard form.
package roman

import (
	"errors"
	"strings"
)

var table = []struct {
	value  int64
	symbol string
}{
	{1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
	{100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
	{10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"},
}

// ErrRange is the error for a number outside 1 to 3999.
var ErrRange = errors.New("roman: n is out of range")

// ErrSyntax is the error for a string that is not a Roman numeral in the standard form.
var ErrSyntax = errors.New("roman: not a Roman numeral in the standard form")

// ToRoman returns the Roman numeral of n, for 1 <= n <= 3999.
func ToRoman(n int64) (string, error) {
	if n < 1 || n > 3999 {
		return "", ErrRange
	}
	var b strings.Builder
	for _, row := range table {
		for n >= row.value {
			b.WriteString(row.symbol)
			n -= row.value
		}
	}
	return b.String(), nil
}

// FromRoman returns the value of a Roman numeral in the standard form.
func FromRoman(s string) (int64, error) {
	if s == "" {
		return 0, ErrSyntax
	}
	var total int64
	for i := 0; i < len(s); i++ {
		v := value(s[i])
		if v == 0 {
			return 0, ErrSyntax
		}
		var next int64
		if i+1 < len(s) {
			next = value(s[i+1])
		}
		if v < next {
			total -= v
		} else {
			total += v
		}
	}
	back, err := ToRoman(total)
	if err != nil || back != s {
		return 0, ErrSyntax
	}
	return total, nil
}

func value(c byte) int64 {
	switch c {
	case 'I':
		return 1
	case 'V':
		return 5
	case 'X':
		return 10
	case 'L':
		return 50
	case 'C':
		return 100
	case 'D':
		return 500
	case 'M':
		return 1000
	}
	return 0
}
