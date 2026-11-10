// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-from-standard-input/.

// Package readlines reads the lines of standard input.
package readlines

import (
	"bufio"
	"io"
	"os"
)

// MaxLine is the longest line, in bytes, that ReadLines accepts. The default limit of bufio.Scanner is 64 KiB.
const MaxLine = 64 << 20

// ReadLines reads r to its end and returns its lines, each without "\n" or "\r\n".
func ReadLines(r io.Reader) ([]string, error) {
	lines := []string{}
	s := bufio.NewScanner(r)
	s.Buffer(make([]byte, 0, 64*1024), MaxLine)
	// Scan returns false at the end of input or on an error.
	for s.Scan() {
		lines = append(lines, s.Text())
	}
	if err := s.Err(); err != nil {
		return nil, err
	}
	return lines, nil
}

// ReadStdinLines reads standard input to its end and returns its lines.
func ReadStdinLines() ([]string, error) {
	return ReadLines(os.Stdin)
}
