// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

package version

import (
	"slices"
	"testing"
)

var cases = []struct {
	name     string
	versions []string
	want     []string
}{
	{"simple", []string{"1.2.0", "1.0.0", "1.1.0"}, []string{"1.0.0", "1.1.0", "1.2.0"}},
	{"fields-in-order", []string{"2.0.0", "1.9.9", "1.10.0"}, []string{"1.9.9", "1.10.0", "2.0.0"}},
	{"patch", []string{"0.1.10", "0.1.2", "0.1.1"}, []string{"0.1.1", "0.1.2", "0.1.10"}},
	{"empty", []string{}, []string{}},
	{"single", []string{"3.4.5"}, []string{"3.4.5"}},
	{"numeric-not-text", []string{"10.0.0", "9.0.0", "1.0.0"}, []string{"1.0.0", "9.0.0", "10.0.0"}},
	{"duplicates", []string{"1.0.0", "0.9.0", "1.0.0"}, []string{"0.9.0", "1.0.0", "1.0.0"}},
	{"zero", []string{"0.0.1", "0.0.0"}, []string{"0.0.0", "0.0.1"}},
	{"limit", []string{"1000000.0.0", "999999.1000000.1000000"}, []string{"999999.1000000.1000000", "1000000.0.0"}},
}

func TestSortVersions(t *testing.T) {
	for _, c := range cases {
		got, err := SortVersions(c.versions)
		if err != nil || !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, %v; want %v", c.name, got, err, c.want)
		}
	}
}

func TestOrderRules(t *testing.T) {
	a, b := Version{1, 2, 3}, Version{1, 10, 0}
	if a.Compare(b) != -1 || b.Compare(a) != 1 || a.Compare(a) != 0 {
		t.Fatal("Compare is not antisymmetric")
	}
	if (a.Compare(Version{1, 2, 3}) == 0) != (a == Version{1, 2, 3}) {
		t.Fatal("Compare is not consistent with ==")
	}
	if _, err := Parse("1.2"); err == nil {
		t.Fatal("Parse accepted 2 parts")
	}
}
