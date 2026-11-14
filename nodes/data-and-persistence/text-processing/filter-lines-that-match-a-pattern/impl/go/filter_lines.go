// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

// Package filterlines selects the lines of a text file that match a regular
// expression.
package filterlines

import (
	"bufio"
	"errors"
	"io"
	"os"
	"regexp"
	"strings"
)

// FilterLines returns the lines of the UTF-8 file at path that match pattern,
// a regular expression in the syntax of package regexp. A line matches if the
// pattern matches some part of it. A line ends at LF, and the LF is not part
// of it. A last line with no LF is a line too. A missing file gives an error
// that matches fs.ErrNotExist.
func FilterLines(path, pattern string) ([]string, error) {
	re, err := regexp.Compile(pattern)
	if err != nil {
		return nil, err
	}
	f, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer f.Close()
	r := bufio.NewReader(f)
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
			if re.MatchString(line) {
				lines = append(lines, line)
			}
		}
		if err != nil {
			return lines, nil
		}
	}
}
