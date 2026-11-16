// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-separators/.

package group

import (
	"math"
	"testing"
)

var cases = []struct {
	name string
	n    int64
	want string
}{
	{"million", 1234567, "1,234,567"},
	{"thousand", 1000, "1,000"},
	{"small", 42, "42"},
	{"zero", 0, "0"},
	{"three-digits", 999, "999"},
	{"negative", -1234567, "-1,234,567"},
	{"negative-small", -999, "-999"},
	{"negative-thousand", -1000, "-1,000"},
	{"int64-max", 9223372036854775807, "9,223,372,036,854,775,807"},
	{"int64-min", math.MinInt64, "-9,223,372,036,854,775,808"},
	{"six-digits", 100000, "100,000"},
	{"seven-digits", 1000000, "1,000,000"},
}

func TestGroupThousands(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := GroupThousands(c.n); got != c.want {
				t.Fatalf("GroupThousands(%d) = %q, want %q", c.n, got, c.want)
			}
		})
	}
}
