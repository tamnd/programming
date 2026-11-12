// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-for-a-substring/.

package search

import "testing"

type searchCase struct {
	name, s, sub string
	want         int
}

// U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
var first = []searchCase{
	{"found", "Hello, world", "world", 7},
	{"first-of-two", "abcabc", "bc", 1},
	{"not-found", "abc", "d", -1},
	{"empty-sub", "abc", "", 0},
	{"empty-s", "", "a", -1},
	{"both-empty", "", "", 0},
	{"longer-sub", "ab", "abc", -1},
	{"overlap", "aaaa", "aa", 0},
	{"case-sensitive", "Hello", "hello", -1},
	{"non-ascii", "naïve caf\u00e9", "caf\u00e9", 6},
	{"cjk", "日本語テキスト", "テキ", 3},
	{"astral", "a\U0001f642b\U0001f642c", "b", 2},
	{"astral-sub", "x\U0001f642y", "\U0001f642y", 1},
	{"combining", "cafe\u0301", "e", 3},
	{"precomposed", "cafe\u0301", "\u00e9", -1},
}

var last = []searchCase{
	{"last-of-two", "abcabc", "bc", 4},
	{"last-not-found", "abc", "d", -1},
	{"last-empty-sub", "abc", "", 3},
	{"last-empty-s", "", "", 0},
	{"last-overlap", "aaaa", "aa", 2},
	{"last-astral", "\U0001f642a\U0001f642a", "a", 3},
	{"last-non-ascii", "caf\u00e9 caf\u00e9", "\u00e9", 8},
}

func TestFind(t *testing.T) {
	for _, c := range first {
		if got := Find(c.s, c.sub); got != c.want {
			t.Errorf("%s: got %d, want %d", c.name, got, c.want)
		}
	}
}

func TestFindLast(t *testing.T) {
	for _, c := range last {
		if got := FindLast(c.s, c.sub); got != c.want {
			t.Errorf("%s: got %d, want %d", c.name, got, c.want)
		}
	}
}
