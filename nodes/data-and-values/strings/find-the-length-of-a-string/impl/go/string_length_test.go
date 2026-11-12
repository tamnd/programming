// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-length-of-a-string/.

package length

import "testing"

func TestLengths(t *testing.T) {
	cases := []struct {
		name, s string
		want    [3]int
	}{
		{"ascii", "hello", [3]int{5, 5, 5}},
		{"words", "Hello, world", [3]int{12, 12, 12}},
		{"two-byte", "naïve", [3]int{6, 5, 5}},
		{"three-byte", "日本語", [3]int{9, 3, 3}},
		{"cyrillic", "Привет", [3]int{12, 6, 6}},
		{"empty", "", [3]int{0, 0, 0}},
		{"single", "a", [3]int{1, 1, 1}},
		{"euro-sign", "€", [3]int{3, 1, 1}},
		{"astral", "🙂", [3]int{4, 2, 1}},
		{"mixed", "a🙂b", [3]int{6, 4, 3}},
		{"combining-mark", "e\u0301", [3]int{3, 2, 2}},
		{"precomposed", "é", [3]int{2, 1, 1}},
		{"flag", "🇫🇷", [3]int{8, 4, 2}},
		{"zwj-family", "👨\u200d👩\u200d👧", [3]int{18, 8, 5}},
		{"nul", "a\x00b", [3]int{3, 3, 3}},
	}
	for _, c := range cases {
		if got := Lengths(c.s); got != c.want {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
