// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-an-integer/.

package parseinteger

import (
	"errors"
	"math"
	"strconv"
	"strings"
	"testing"
)

func TestParseInteger(t *testing.T) {
	cases := []struct {
		name, s string
		base    int
		want    int64
	}{
		{"decimal", "42", 10, 42},
		{"negative", "-17", 10, -17},
		{"hex", "ff", 16, 255},
		{"binary", "101010", 2, 42},
		{"base-36", "zz", 36, 1295},
		{"plus-sign", "+5", 10, 5},
		{"negative-zero", "-0", 10, 0},
		{"zero", "0", 10, 0},
		{"leading-zeros", "007", 10, 7},
		{"many-leading-zeros", strings.Repeat("0", 5000) + "42", 10, 42},
		{"max", "9223372036854775807", 10, 9223372036854775807},
		{"min", "-9223372036854775808", 10, math.MinInt64},
		{"max-hex", "7fffffffffffffff", 16, 9223372036854775807},
		{"min-binary", "-1000000000000000000000000000000000000000000000000000000000000000", 2, math.MinInt64},
		{"upper-case", "FF", 16, 255},
		{"mixed-case", "Zz", 36, 1295},
		{"above-safe-integer", "9007199254740993", 10, 9007199254740993},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got, err := ParseInteger(c.s, c.base)
			if err != nil || got != c.want {
				t.Errorf("got %d, %v; want %d", got, err, c.want)
			}
		})
	}
}

func TestParseIntegerErrors(t *testing.T) {
	cases := []struct {
		name, s string
		base    int
		want    error
	}{
		{"empty", "", 10, strconv.ErrSyntax},
		{"sign-only", "-", 10, strconv.ErrSyntax},
		{"plus-only", "+", 10, strconv.ErrSyntax},
		{"double-sign", "+-5", 10, strconv.ErrSyntax},
		{"leading-space", " 42", 10, strconv.ErrSyntax},
		{"trailing-space", "42 ", 10, strconv.ErrSyntax},
		{"trailing-newline", "42\n", 10, strconv.ErrSyntax},
		{"trailing-garbage", "42px", 10, strconv.ErrSyntax},
		{"underscore", "1_000", 10, strconv.ErrSyntax},
		{"thousands-separator", "1,000", 10, strconv.ErrSyntax},
		{"decimal-point", "4.0", 10, strconv.ErrSyntax},
		{"exponent", "1e3", 10, strconv.ErrSyntax},
		{"hex-prefix", "0x1A", 16, strconv.ErrSyntax},
		{"binary-prefix", "0b101", 2, strconv.ErrSyntax},
		{"digit-outside-base", "102", 2, strconv.ErrSyntax},
		{"letter-outside-base", "fg", 16, strconv.ErrSyntax},
		{"arabic-indic-digits", "\u0661\u0662", 10, strconv.ErrSyntax},
		{"fullwidth-digits", "\uff11\uff12", 10, strconv.ErrSyntax},
		{"kelvin-sign", "\u212a", 36, strconv.ErrSyntax},
		{"max-plus-one", "9223372036854775808", 10, strconv.ErrRange},
		{"min-minus-one", "-9223372036854775809", 10, strconv.ErrRange},
		{"two-to-the-64", "18446744073709551616", 10, strconv.ErrRange},
		{"hex-sign-bit", "8000000000000000", 16, strconv.ErrRange},
		{"hex-all-ones", "ffffffffffffffff", 16, strconv.ErrRange},
		{"min-times-ten", "-92233720368547758080", 10, strconv.ErrRange},
		{"many-digits", strings.Repeat("9", 5000), 10, strconv.ErrRange},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			_, err := ParseInteger(c.s, c.base)
			if !errors.Is(err, c.want) {
				t.Errorf("got error %v; want %v", err, c.want)
			}
		})
	}
}
