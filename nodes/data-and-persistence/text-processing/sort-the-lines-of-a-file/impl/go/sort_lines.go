// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

// Package sortlines sorts the lines of a text file.
package sortlines

import (
	"os"
	"slices"
	"strings"
)

// SortLines returns the lines of the UTF-8 file at path, sorted by code point.
// A line ends at LF, and the LF is not part of it. A last line with no LF is a
// line too. A missing file gives an error that matches fs.ErrNotExist.
func SortLines(path string) ([]string, error) {
	data, err := os.ReadFile(path)
	if err != nil {
		return nil, err
	}
	lines := strings.Split(string(data), "\n")
	// After the LF at the end of the last line, Split gives one empty string.
	// An empty file gives [""] too, which is no lines.
	if lines[len(lines)-1] == "" {
		lines = lines[:len(lines)-1]
	}
	// Go compares strings byte by byte, and the order of the UTF-8 bytes is
	// the order of the code points.
	slices.Sort(lines)
	return lines, nil
}
