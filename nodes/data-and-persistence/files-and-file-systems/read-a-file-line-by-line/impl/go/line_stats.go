// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-file-line-by-line/.

// Package linestats reads a text file line by line.
package linestats

import (
	"bufio"
	"errors"
	"fmt"
	"os"
	"unicode/utf8"
)

// MaxLine is the longest line, in bytes, that LineStats accepts. The default limit of bufio.Scanner is 64 KiB.
const MaxLine = 64 << 20

// ErrInvalidUTF8 reports a line that is not valid UTF-8.
var ErrInvalidUTF8 = errors.New("invalid UTF-8")

// LineStats returns the number of lines of a UTF-8 text file and the number of code points in its longest line.
func LineStats(path string) (lines, longest int, err error) {
	f, err := os.Open(path)
	if err != nil {
		return 0, 0, err
	}
	defer f.Close()

	s := bufio.NewScanner(f)
	s.Buffer(make([]byte, 0, 64*1024), MaxLine)
	for s.Scan() {
		line := s.Bytes() // without "\n" or "\r\n"; valid until the next Scan
		lines++
		if !utf8.Valid(line) {
			return 0, 0, fmt.Errorf("%s: line %d: %w", path, lines, ErrInvalidUTF8)
		}
		longest = max(longest, utf8.RuneCount(line))
	}
	if err := s.Err(); err != nil {
		return 0, 0, fmt.Errorf("%s: line %d: %w", path, lines+1, err)
	}
	return lines, longest, nil
}
