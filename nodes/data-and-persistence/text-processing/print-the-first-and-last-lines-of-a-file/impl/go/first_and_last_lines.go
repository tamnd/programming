// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

// Package firstandlast gets the first lines and the last lines of a text file
// in one pass.
package firstandlast

import (
	"bufio"
	"errors"
	"io"
	"os"
	"strings"
)

// FirstAndLastLines returns the first count lines and the last count lines of
// the UTF-8 file at path. If the file has fewer than count lines, each slice
// has all the lines. A line ends at LF, and the LF is not part of it. A last
// line with no LF is a line too. A missing file gives an error that matches
// fs.ErrNotExist.
func FirstAndLastLines(path string, count int) (first, last []string, err error) {
	f, err := os.Open(path)
	if err != nil {
		return nil, nil, err
	}
	defer f.Close()
	r := bufio.NewReader(f)
	first = []string{}
	// ring holds the last count lines. The line number n goes to ring[n%count],
	// over the line that came count lines before it.
	ring := make([]string, count)
	n := 0
	for {
		// ReadString returns the line with its LF, or the last line with no
		// LF together with io.EOF.
		line, err := r.ReadString('\n')
		if err != nil && !errors.Is(err, io.EOF) {
			return nil, nil, err
		}
		if line != "" && count > 0 {
			line = strings.TrimSuffix(line, "\n")
			if n < count {
				first = append(first, line)
			}
			ring[n%count] = line
			n++
		}
		if err != nil {
			break
		}
	}
	// The oldest line in the ring is at n%count, if the ring is full.
	last = make([]string, 0, min(n, count))
	for i := max(n-count, 0); i < n; i++ {
		last = append(last, ring[i%count])
	}
	return first, last, nil
}
