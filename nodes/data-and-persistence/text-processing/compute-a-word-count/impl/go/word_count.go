// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-word-count/.

// Package wordcount counts the lines, words and bytes of a UTF-8 file, as wc does.
package wordcount

import (
	"bufio"
	"errors"
	"fmt"
	"io"
	"os"
	"unicode"
	"unicode/utf8"
)

// ErrInvalidUTF8 reports bytes that are not valid UTF-8.
var ErrInvalidUTF8 = errors.New("invalid UTF-8")

// Counts holds the result: Lines is the number of LF bytes, and a word is a
// run of one or more runes that are not white space.
type Counts struct {
	Lines, Words, Bytes int64
}

// Count reads UTF-8 text from r. unicode.IsSpace is true for the runes that
// have the Unicode property White_Space.
func Count(r io.Reader) (Counts, error) {
	var c Counts
	br := bufio.NewReaderSize(r, 64*1024)
	inWord := false
	for {
		ch, size, err := br.ReadRune()
		if errors.Is(err, io.EOF) {
			return c, nil
		}
		if err != nil {
			return Counts{}, err
		}
		// ReadRune gives U+FFFD with size 1 for a bad byte. A real U+FFFD has size 3.
		if ch == utf8.RuneError && size == 1 {
			return Counts{}, fmt.Errorf("%w at byte %d", ErrInvalidUTF8, c.Bytes)
		}
		c.Bytes += int64(size)
		switch {
		case unicode.IsSpace(ch):
			inWord = false
			if ch == '\n' {
				c.Lines++
			}
		case !inWord:
			inWord = true
			c.Words++
		}
	}
}

// CountFile counts the lines, words and bytes of the file at path.
func CountFile(path string) (Counts, error) {
	f, err := os.Open(path)
	if err != nil {
		return Counts{}, err
	}
	defer f.Close()
	return Count(f)
}
