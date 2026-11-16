// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-anagrams/.

package anagrams

import "testing"

var cases = []struct {
	name string
	a    string
	b    string
	want bool
}{
	{"listen-silent", "listen", "silent", true},
	{"dormitory", "Dormitory", "dirty room", true},
	{"hello-world", "hello", "world", false},
	{"both-empty", "", "", true},
	{"one-empty", "a", "", false},
	{"same-set-other-counts", "aab", "abb", false},
	{"extra-letter", "abc", "abcd", false},
	{"same-length-other-letters", "abc", "abd", false},
	{"case", "Listen", "SILENT", true},
	{"same-word", "stop", "stop", true},
	{"gentleman", "a gentleman", "elegant man", true},
	{"they-see", "The eyes!", "They see.", true},
	{"punctuation-only", "!?", "...", true},
}

func TestAreAnagrams(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := AreAnagrams(c.a, c.b); got != c.want {
				t.Fatalf("AreAnagrams(%q, %q) = %v, want %v", c.a, c.b, got, c.want)
			}
			if got := AreAnagrams(c.b, c.a); got != c.want {
				t.Fatalf("AreAnagrams(%q, %q) = %v, want %v", c.b, c.a, got, c.want)
			}
		})
	}
}
