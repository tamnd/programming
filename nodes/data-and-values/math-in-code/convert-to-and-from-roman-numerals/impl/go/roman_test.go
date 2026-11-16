// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

package roman

import (
	"errors"
	"testing"
)

var cases = []struct {
	name string
	n    int64
	want string
}{
	{"year-1990", 1990, "MCMXC"},
	{"year-2008", 2008, "MMVIII"},
	{"each-symbol-once", 1666, "MDCLXVI"},
	{"one", 1, "I"},
	{"four", 4, "IV"},
	{"nine", 9, "IX"},
	{"forty-nine", 49, "XLIX"},
	{"ninety-nine", 99, "XCIX"},
	{"four-hundred-forty-four", 444, "CDXLIV"},
	{"year-1994", 1994, "MCMXCIV"},
	{"longest", 3888, "MMMDCCCLXXXVIII"},
	{"largest", 3999, "MMMCMXCIX"},
}

func TestToRoman(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got, err := ToRoman(c.n)
			if err != nil || got != c.want {
				t.Fatalf("ToRoman(%d) = %q, %v, want %q", c.n, got, err, c.want)
			}
			back, err := FromRoman(c.want)
			if err != nil || back != c.n {
				t.Fatalf("FromRoman(%q) = %d, %v", c.want, back, err)
			}
		})
	}
}

func TestRoundTrip(t *testing.T) {
	for n := int64(1); n <= 3999; n++ {
		s, err := ToRoman(n)
		if err != nil {
			t.Fatal(err)
		}
		if back, err := FromRoman(s); err != nil || back != n {
			t.Fatalf("FromRoman(%q) = %d, %v", s, back, err)
		}
	}
}

func TestInvalid(t *testing.T) {
	for _, s := range []string{"", "IIII", "IC", "VX", "MMMM", "iv", "ABC", "IIV", "XM"} {
		if _, err := FromRoman(s); !errors.Is(err, ErrSyntax) {
			t.Fatalf("FromRoman(%q): err = %v", s, err)
		}
	}
	for _, n := range []int64{0, -1, 4000} {
		if _, err := ToRoman(n); !errors.Is(err, ErrRange) {
			t.Fatalf("ToRoman(%d): err = %v", n, err)
		}
	}
}
