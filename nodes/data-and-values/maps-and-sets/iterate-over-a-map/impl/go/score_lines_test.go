// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-a-map/.

package scorelines

import (
	"slices"
	"testing"
)

var cases = []struct {
	name   string
	scores map[string]int
	want   []string
}{
	{"scores", map[string]int{"bob": 7, "amy": 9, "cal": 4}, []string{"amy=9", "bob=7", "cal=4", "total=20"}},
	{"one", map[string]int{"x": 1}, []string{"x=1", "total=1"}},
	{"empty", map[string]int{}, []string{"total=0"}},
	{"case", map[string]int{"b": 1, "B": 2, "a": 3, "A": 4}, []string{"A=4", "B=2", "a=3", "b=1", "total=10"}},
	{"negative", map[string]int{"a": -5, "b": 5}, []string{"a=-5", "b=5", "total=0"}},
	{"prefix", map[string]int{"abc": 3, "a": 2, "ab": 1}, []string{"a=2", "ab=1", "abc=3", "total=6"}},
	{"reverse", map[string]int{"j": 1, "i": 2, "h": 3, "g": 4, "f": 5, "e": 6, "d": 7, "c": 8, "b": 9, "a": 10}, []string{"a=10", "b=9", "c=8", "d=7", "e=6", "f=5", "g=4", "h=3", "i=2", "j=1", "total=55"}},
}

func TestScoreLines(t *testing.T) {
	for _, c := range cases {
		if got := ScoreLines(c.scores); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
