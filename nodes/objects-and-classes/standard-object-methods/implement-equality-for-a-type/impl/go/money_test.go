// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-equality-for-a-type/.

package money

import "testing"

var cases = []struct {
	name       string
	amounts    []int64
	currencies []string
	want       int
}{
	{"two-equal", []int64{1250, 1250}, []string{"EUR", "EUR"}, 1},
	{"different-amounts", []int64{100, 200, 100}, []string{"USD", "USD", "USD"}, 2},
	{"mixed", []int64{500, 500, 500, 700}, []string{"EUR", "USD", "EUR", "EUR"}, 3},
	{"empty", []int64{}, []string{}, 0},
	{"case", []int64{999, 999, 999}, []string{"gbp", "GBP", "Gbp"}, 1},
	{"other-currency", []int64{0, 0, 0}, []string{"EUR", "USD", "JPY"}, 3},
	{"negative", []int64{-5, 5, -5}, []string{"EUR", "EUR", "EUR"}, 2},
	{"limit", []int64{1e12, -1e12, 1e12}, []string{"usd", "USD", "USD"}, 2},
}

func TestCountDistinct(t *testing.T) {
	for _, c := range cases {
		if got := CountDistinct(c.amounts, c.currencies); got != c.want {
			t.Errorf("%s: got %d, want %d", c.name, got, c.want)
		}
	}
}

func TestEqualityRules(t *testing.T) {
	a, b := New(1250, "EUR"), New(1250, "eur")
	if !a.Equal(a) || !a.Equal(b) || !b.Equal(a) || a != b {
		t.Fatal("equal values are not equal")
	}
	if a.Equal(New(1251, "EUR")) || a.Equal(New(1250, "USD")) {
		t.Fatal("different values are equal")
	}
	names := map[Money]string{New(500, "USD"): "lunch"}
	if names[New(500, "usd")] != "lunch" {
		t.Fatal("map lookup with an equal key failed")
	}
}
