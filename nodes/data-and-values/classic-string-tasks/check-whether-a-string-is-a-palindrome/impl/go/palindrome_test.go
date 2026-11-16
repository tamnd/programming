// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-a-palindrome/.

package palindrome

import "testing"

var cases = []struct {
	name string
	text string
	want bool
}{
	{"racecar", "racecar", true},
	{"panama", "A man, a plan, a canal: Panama", true},
	{"hello", "hello", false},
	{"empty", "", true},
	{"only-punctuation", "?! ...", true},
	{"one-letter", "a", true},
	{"two-letters", "ab", false},
	{"mixed-case", "Aa", true},
	{"nixon", "No 'x' in Nixon", true},
	{"cat-i-saw", "Was it a car or a cat I saw?", true},
	{"digits", "12321", true},
	{"digits-not", "123", false},
	{"zero-p", "0P", false},
	{"race-a-car", "race a car", false},
	{"even-near", "abca", false},
}

func TestIsPalindrome(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := IsPalindrome(c.text); got != c.want {
				t.Fatalf("IsPalindrome(%q) = %v, want %v", c.text, got, c.want)
			}
		})
	}
}
