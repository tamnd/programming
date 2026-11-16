// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-record-type/.

package birthday

import (
	"fmt"
	"slices"
	"testing"
)

var cases = []struct {
	name   string
	people []Person
	who    string
	want   []Person
}{
	{"first", []Person{{"Ada", 36}, {"Alan", 41}}, "Ada", []Person{{"Ada", 37}, {"Alan", 41}}},
	{"second", []Person{{"Ada", 36}, {"Alan", 41}}, "Alan", []Person{{"Ada", 36}, {"Alan", 42}}},
	{"empty", []Person{}, "Ada", []Person{}},
	{"not-found", []Person{{"Ada", 36}}, "Grace", []Person{{"Ada", 36}}},
	{"duplicate-name", []Person{{"Ada", 36}, {"Alan", 41}, {"Ada", 9}}, "Ada", []Person{{"Ada", 37}, {"Alan", 41}, {"Ada", 10}}},
	{"zero-age", []Person{{"Baby", 0}}, "Baby", []Person{{"Baby", 1}}},
	{"non-ascii", []Person{{"Zoë", 20}, {"Zoe", 30}}, "Zoë", []Person{{"Zoë", 21}, {"Zoe", 30}}},
	{"case", []Person{{"ada", 36}}, "Ada", []Person{{"ada", 36}}},
}

func TestBirthday(t *testing.T) {
	for _, c := range cases {
		before := slices.Clone(c.people)
		if got := Birthday(c.people, c.who); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
		if !slices.Equal(c.people, before) {
			t.Errorf("%s: the input changed", c.name)
		}
	}
}

func TestEqualityAndPrinting(t *testing.T) {
	if (Person{"Ada", 36}) != (Person{"Ada", 36}) {
		t.Error("equal fields must give equal structs")
	}
	if got := fmt.Sprintf("%+v", Person{"Ada", 36}); got != "{Name:Ada Age:36}" {
		t.Errorf("got %q", got)
	}
}
