// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-set/.

package toggle

import (
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	words []string
	want  []string
}{
	{"colors", []string{"red", "blue", "red", "green"}, []string{"blue", "green"}},
	{"fresh", []string{"b", "a"}, []string{"a", "b"}},
	{"empty", []string{}, []string{}},
	{"two-times", []string{"a", "a"}, []string{}},
	{"three-times", []string{"a", "a", "a"}, []string{"a"}},
	{"case", []string{"a", "A"}, []string{"A", "a"}},
	{"prefix", []string{"ab", "a"}, []string{"a", "ab"}},
}

func TestToggle(t *testing.T) {
	for _, c := range cases {
		if got := Toggle(c.words); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}
