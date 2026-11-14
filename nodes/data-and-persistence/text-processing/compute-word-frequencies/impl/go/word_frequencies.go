// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-word-frequencies/.

// Package wordfreq finds the most frequent words of a text file.
package wordfreq

import (
	"bufio"
	"cmp"
	"errors"
	"io"
	"os"
	"slices"
	"strings"
)

// WordCount is one word and the number of times it occurs.
type WordCount struct {
	Word  string
	Count int
}

// WordFrequencies returns the n most frequent words of the file at path. A
// word is a run of ASCII letters, changed to lower case. The result is sorted
// by count from high to low, then by word. A missing file gives an error that
// matches fs.ErrNotExist.
func WordFrequencies(path string, n int) ([]WordCount, error) {
	f, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer f.Close()
	counts := map[string]int{}
	r := bufio.NewReaderSize(f, 64*1024)
	// The bytes of the current word. They stay across the reads of the buffer,
	// so a word that the end of a buffer cuts is not split.
	var word []byte
	for {
		b, err := r.ReadByte()
		if err != nil && !errors.Is(err, io.EOF) {
			return nil, err
		}
		switch {
		case err == nil && 'a' <= b && b <= 'z':
			word = append(word, b)
		case err == nil && 'A' <= b && b <= 'Z':
			word = append(word, b+'a'-'A')
		case len(word) > 0:
			// A byte that is not a letter, or the end of the file, ends the word.
			counts[string(word)]++
			word = word[:0]
		}
		if err != nil {
			break
		}
	}
	result := make([]WordCount, 0, len(counts))
	for w, c := range counts {
		result = append(result, WordCount{w, c})
	}
	// The order of a map is not fixed, so the word is the second key.
	slices.SortFunc(result, func(a, b WordCount) int {
		return cmp.Or(cmp.Compare(b.Count, a.Count), strings.Compare(a.Word, b.Word))
	})
	return result[:min(n, len(result))], nil
}
