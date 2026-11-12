// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/join-a-list-of-strings/.

package join

import "testing"

func TestJoin(t *testing.T) {
	cases := []struct {
		name  string
		items []string
		sep   string
		want  string
	}{
		{"comma", []string{"a", "b", "c"}, ",", "a,b,c"},
		{"comma-space", []string{"red", "green", "blue"}, ", ", "red, green, blue"},
		{"words", []string{"Hello", "world"}, " ", "Hello world"},
		{"empty-list", []string{}, ",", ""},
		{"single", []string{"only"}, ",", "only"},
		{"empty-items", []string{"", "", ""}, ",", ",,"},
		{"empty-middle", []string{"a", "", "b"}, "-", "a--b"},
		{"empty-sep", []string{"a", "b", "c"}, "", "abc"},
		{"long-sep", []string{"x", "y"}, " -> ", "x -> y"},
		{"non-ascii", []string{"naïve", "日本"}, "·", "naïve·日本"},
		{"astral-sep", []string{"a", "b"}, "\U0001F642", "a\U0001F642b"},
		{"sep-in-item", []string{"a,b", "c"}, ",", "a,b,c"},
	}
	for _, c := range cases {
		if got := Join(c.items, c.sep); got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}

func TestJoinNumbers(t *testing.T) {
	cases := []struct {
		name  string
		items []int32
		sep   string
		want  string
	}{
		{"numbers", []int32{1, 2, 3}, ",", "1,2,3"},
		{"negative", []int32{-1, 0, 1}, " ", "-1 0 1"},
		{"empty", []int32{}, ",", ""},
		{"single", []int32{42}, ",", "42"},
		{"int32-ends", []int32{2147483647, -2147483648}, ";", "2147483647;-2147483648"},
	}
	for _, c := range cases {
		if got := JoinNumbers(c.items, c.sep); got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}
