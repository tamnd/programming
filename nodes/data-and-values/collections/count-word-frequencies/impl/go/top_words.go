// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-word-frequencies/.

// Package wordfreq counts the words of a text.
package wordfreq

import (
	"cmp"
	"slices"
	"strings"
	"unicode/utf8"
)

// WordCount is a word and the number of times it occurs.
type WordCount struct {
	Word  string
	Count int
}

// isSeparator tells if r ends a word: an ASCII character that is not a letter
// or a digit.
func isSeparator(r rune) bool {
	if r >= utf8.RuneSelf {
		return false
	}
	return !('a' <= r && r <= 'z' || 'A' <= r && r <= 'Z' || '0' <= r && r <= '9')
}

// TopWords returns the n most frequent words of text, by count from high to
// low, then by word in code point order.
func TopWords(text string, n int) []WordCount {
	counts := make(map[string]int)
	for _, word := range strings.FieldsFunc(text, isSeparator) {
		counts[strings.ToLower(word)]++
	}
	// The iteration order of a map is not specified, so sort by both keys.
	out := make([]WordCount, 0, len(counts))
	for word, count := range counts {
		out = append(out, WordCount{word, count})
	}
	// Strings compare by bytes. For UTF-8 that is code point order.
	slices.SortFunc(out, func(a, b WordCount) int {
		return cmp.Or(cmp.Compare(b.Count, a.Count), strings.Compare(a.Word, b.Word))
	})
	return out[:min(n, len(out))]
}
