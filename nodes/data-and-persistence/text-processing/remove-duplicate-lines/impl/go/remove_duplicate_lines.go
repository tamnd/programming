// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicate-lines/.

// Package dedup removes the duplicate lines of a text file.
package dedup

import (
	"bufio"
	"errors"
	"io"
	"os"
	"strings"
)

// RemoveDuplicateLines returns the lines of the UTF-8 file at path, each one
// only once, at the place of its first copy. A line ends at LF, and the LF is
// not part of it. A last line with no LF is a line too. Memory grows with the
// different lines, not with the size of the file. A missing file gives an
// error that matches fs.ErrNotExist.
func RemoveDuplicateLines(path string) ([]string, error) {
	f, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer f.Close()
	r := bufio.NewReader(f)
	seen := map[string]bool{}
	lines := []string{}
	for {
		// ReadString returns the line with its LF, or the last line with no
		// LF together with io.EOF.
		line, err := r.ReadString('\n')
		if err != nil && !errors.Is(err, io.EOF) {
			return nil, err
		}
		if line != "" {
			line = strings.TrimSuffix(line, "\n")
			if !seen[line] {
				seen[line] = true
				lines = append(lines, line)
			}
		}
		if err != nil {
			return lines, nil
		}
	}
}
