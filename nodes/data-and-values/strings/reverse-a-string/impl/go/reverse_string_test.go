// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-string/.

package reversestring

import (
	"errors"
	"testing"
)

func TestCases(t *testing.T) {
	cases := []struct{ name, s, want string }{
		{"ascii", "hello", "olleh"},
		{"two-words", "abc def", "fed cba"},
		{"palindrome", "racecar", "racecar"},
		{"empty", "", ""},
		{"single", "a", "a"},
		{"two-byte", "naïve", "evïan"},
		{"three-byte", "日本語", "語本日"},
		{"astral", "a🙂b", "b🙂a"},
		{"two-astral", "🙂😀", "😀🙂"},
		{"combining-mark", "e\u0301x", "x\u0301e"},
		{"flag", "🇫🇷", "🇷🇫"},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got, err := ReverseCodePoints(c.s)
			if err != nil || got != c.want {
				t.Errorf("ReverseCodePoints(%q) = %q, %v; want %q, nil", c.s, got, err, c.want)
			}
		})
	}
}

func TestInvalidUTF8(t *testing.T) {
	for name, s := range map[string]string{
		"invalid-byte":      "\xff\xfe",
		"truncated":         "caf\xc3",
		"encoded-surrogate": "a\xed\xa0\x80b",
		"overlong":          "\xc0\xaf",
	} {
		t.Run(name, func(t *testing.T) {
			if got, err := ReverseCodePoints(s); !errors.Is(err, ErrInvalidUTF8) || got != "" {
				t.Errorf("ReverseCodePoints(%q) = %q, %v; want \"\", ErrInvalidUTF8", s, got, err)
			}
		})
	}
}
