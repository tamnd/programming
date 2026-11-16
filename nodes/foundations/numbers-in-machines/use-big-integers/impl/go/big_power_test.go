// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-big-integers/.

package bigpow

import (
	"math"
	"math/big"
	"strings"
	"testing"
)

var cases = []struct {
	name      string
	base, exp int
	want      string
}{
	{"small", 2, 10, "1024"},
	{"two-to-the-100", 2, 100, "1267650600228229401496703205376"},
	{"ten-to-the-30", 10, 30, "1" + strings.Repeat("0", 30)},
	{"zero-exponent", 7, 0, "1"},
	{"zero-to-the-zero", 0, 0, "1"},
	{"zero-base", 0, 5, "0"},
	{"minus-one-odd", -1, 199, "-1"},
	{"negative-odd", -3, 41, "-36472996377170786403"},
	{"negative-even", -3, 40, "12157665459056928801"},
	{"int64-max-plus-one", 2, 63, "9223372036854775808"},
	{"uint64-max-plus-one", 2, 64, "18446744073709551616"},
	{"two-to-the-200", 2, 200, "1606938044258990275541962092341162602522202993782792835301376"},
	{"largest", 1000, 200, "1" + strings.Repeat("0", 600)},
	{"smallest", -1000, 199, "-1" + strings.Repeat("0", 597)},
}

func TestBigPower(t *testing.T) {
	for _, c := range cases {
		if got := BigPower(c.base, c.exp); got != c.want {
			t.Errorf("%s: got %s, want %s", c.name, got, c.want)
		}
	}
}

func TestFixedWidth(t *testing.T) {
	big63 := new(big.Int).Lsh(big.NewInt(1), 63)
	if big63.IsInt64() {
		t.Error("2^63 fits in an int64")
	}
	max := new(big.Int).Sub(big63, big.NewInt(1))
	if !max.IsInt64() || max.Int64() != math.MaxInt64 {
		t.Error("2^63 - 1 is not math.MaxInt64")
	}
	if !big63.IsUint64() || big63.Uint64() != 1<<63 {
		t.Error("2^63 does not fit in a uint64")
	}
}
